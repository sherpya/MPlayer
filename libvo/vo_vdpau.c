/*
 * VDPAU video output driver
 *
 * Copyright (C) 2008 NVIDIA
 *
 * This file is part of MPlayer.
 *
 * MPlayer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * MPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with MPlayer; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/**
 * \defgroup  VDPAU_Presentation VDPAU Presentation
 * \ingroup Decoder
 *
 * Actual decoding and presentation are implemented here.
 * All necessary frame information is collected through
 * the "vdpau_render_state" structure after parsing all headers
 * etc. in libavcodec for different codecs.
 *
 * @{
 */

#include <stdio.h>
#include <vdpau/vdpau_x11.h>

#include "config.h"
#include "sub/ass_mp.h"
#include "mp_msg.h"
#include "video_out.h"
#define NO_DRAW_FRAME
#include "video_out_internal.h"
#include "libmpcodecs/vdpau_frame_data.h"
#include "libmpcodecs/vf.h"
#include "x11_common.h"
#include "aspect.h"
#include "sub/font_load.h"
#include "sub/sub.h"
#include "sub/eosd.h"
#include "subopt-helper.h"

#include "libavcodec/vdpau.h"

#include "libavutil/common.h"
#include "libavutil/avstring.h"
#include "libavutil/mathematics.h"


static vo_info_t info = {
    "VDPAU with X11",
    "vdpau",
    "Rajib Mahapatra <rmahapatra@nvidia.com> and others",
    ""
};

const LIBVO_EXTERN(vdpau)

#define CHECK_ST_ERROR(message) \
    if (vdp_st != VDP_STATUS_OK) { \
        mp_msg(MSGT_VO, MSGL_ERR, "[vdpau] %s: %s\n", \
               message, vdp_get_error_string(vdp_st)); \
        return -1; \
    }

#define CHECK_ST_WARNING(message) \
    if (vdp_st != VDP_STATUS_OK) \
        mp_msg(MSGT_VO, MSGL_WARN, "[vdpau] %s: %s\n", \
               message, vdp_get_error_string(vdp_st));

/* number of video and output surfaces */
#define NUM_OUTPUT_SURFACES                3
#define MAX_VIDEO_SURFACES                 50

/* number of palette entries */
#define PALETTE_SIZE 256

/* Initial maximum number of EOSD surfaces */
#define EOSD_SURFACES_INITIAL 512

/*
 * Global variable declaration - VDPAU specific
 */

/* Declaration for all variables of win_x11_init_vdpau_procs() and
 * win_x11_init_vdpau_flip_queue() functions
 */
static VdpDevice                          vdp_device;
static VdpGetProcAddress                 *vdp_get_proc_address;

static VdpPresentationQueueTarget         vdp_flip_target;
static VdpPresentationQueue               vdp_flip_queue;

static VdpDeviceDestroy                  *vdp_device_destroy;
static VdpVideoSurfaceCreate             *vdp_video_surface_create;
static VdpVideoSurfaceDestroy            *vdp_video_surface_destroy;

static VdpGetErrorString                 *vdp_get_error_string;

/* May be used in software filtering/postprocessing options
 * in MPlayer (./mplayer -vf ..) if we copy video_surface data to
 * system memory.
 */
static VdpVideoSurfacePutBitsYCbCr       *vdp_video_surface_put_bits_y_cb_cr;
static VdpOutputSurfacePutBitsNative     *vdp_output_surface_put_bits_native;

static VdpOutputSurfaceCreate            *vdp_output_surface_create;
static VdpOutputSurfaceDestroy           *vdp_output_surface_destroy;

/* VideoMixer puts video_surface data on displayable output_surface. */
static VdpVideoMixerCreate               *vdp_video_mixer_create;
static VdpVideoMixerDestroy              *vdp_video_mixer_destroy;
static VdpVideoMixerRender               *vdp_video_mixer_render;
static VdpVideoMixerSetFeatureEnables    *vdp_video_mixer_set_feature_enables;
static VdpVideoMixerSetAttributeValues   *vdp_video_mixer_set_attribute_values;

static VdpPresentationQueueTargetDestroy *vdp_presentation_queue_target_destroy;
static VdpPresentationQueueCreate        *vdp_presentation_queue_create;
static VdpPresentationQueueDestroy       *vdp_presentation_queue_destroy;
static VdpPresentationQueueDisplay       *vdp_presentation_queue_display;
static VdpPresentationQueueBlockUntilSurfaceIdle *vdp_presentation_queue_block_until_surface_idle;
static VdpPresentationQueueTargetCreateX11       *vdp_presentation_queue_target_create_x11;
static VdpPresentationQueueSetBackgroundColor    *vdp_presentation_queue_set_background_color;

static VdpOutputSurfaceRenderOutputSurface       *vdp_output_surface_render_output_surface;
static VdpOutputSurfacePutBitsIndexed            *vdp_output_surface_put_bits_indexed;
static VdpOutputSurfaceRenderBitmapSurface       *vdp_output_surface_render_bitmap_surface;

static VdpBitmapSurfaceCreate                    *vdp_bitmap_surface_create;
static VdpBitmapSurfaceDestroy                   *vdp_bitmap_surface_destroy;
static VdpBitmapSurfacePutBitsNative             *vdp_bitmap_surface_putbits_native;

static VdpDecoderCreate                          *vdp_decoder_create;
static VdpDecoderDestroy                         *vdp_decoder_destroy;
static VdpDecoderRender                          *vdp_decoder_render;

static VdpGenerateCSCMatrix                      *vdp_generate_csc_matrix;
static VdpPreemptionCallbackRegister             *vdp_preemption_callback_register;

/* output_surfaces[NUM_OUTPUT_SURFACES] is misused for OSD. */
#define osd_surface output_surfaces[NUM_OUTPUT_SURFACES]
static VdpOutputSurface                   output_surfaces[NUM_OUTPUT_SURFACES + 1];
static VdpOutputSurface                   rgba_surface;
static VdpVideoSurface                    deint_surfaces[3];
static mp_image_t                        *deint_mpi[2];
static int                                output_surface_width, output_surface_height;

static VdpVideoMixer                      video_mixer;
static int                                deint;
static int                                deint_type;
static int                                deint_counter;
static int                                pullup;
static float                              denoise;
static float                              sharpen;
static int                                colorspace;
static int                                chroma_deint;
static int                                force_mixer;
static int                                top_field_first;
static int                                flip;
static int                                hqscaling;

static VdpDecoder                         decoder;
static int                                decoder_max_refs;

static VdpRect                            src_rect_vid;
static VdpRect                            out_rect_vid;
static int                                border_l, border_r, border_t, border_b;

static VdpVideoSurface                    surface_render[MAX_VIDEO_SURFACES];
static int                                surface_num;
static int                                vid_surface_num;
static uint32_t                           vid_width, vid_height;
static uint32_t                           image_format;
static VdpChromaType                      vdp_chroma_type;
static VdpYCbCrFormat                     vdp_pixel_format;

static volatile int                       is_preempted;

/* draw_osd */
static unsigned char                     *index_data;
static int                                index_data_size;
static uint32_t                           palette[PALETTE_SIZE];

// EOSD
// Pool of surfaces
struct {
    VdpBitmapSurface surface;
    int w;
    int h;
    char in_use;
} *eosd_surfaces;

// List of surfaces to be rendered
struct {
    VdpBitmapSurface surface;
    VdpRect source;
    VdpRect dest;
    VdpColor color;
} *eosd_targets;

static int eosd_render_count;
static int eosd_surface_count;

// Video equalizer
static VdpProcamp procamp;

/*
 * X11 specific
 */
static int                                visible_buf;

static void draw_eosd(void);

static void push_deint_surface(VdpVideoSurface surface)
{
    deint_surfaces[2] = deint_surfaces[1];
    deint_surfaces[1] = deint_surfaces[0];
    deint_surfaces[0] = surface;
}

static const uint32_t rotate_flags[4] = {
    VDP_OUTPUT_SURFACE_RENDER_ROTATE_0,
    VDP_OUTPUT_SURFACE_RENDER_ROTATE_90,
    VDP_OUTPUT_SURFACE_RENDER_ROTATE_180,
    VDP_OUTPUT_SURFACE_RENDER_ROTATE_270
};

static void video_to_output_surface(void)
{
    VdpTime dummy;
    VdpStatus vdp_st;
    int i;
    if (image_format == IMGFMT_BGRA) {
        vdp_st = vdp_output_surface_render_output_surface(output_surfaces[surface_num],
                                                          &out_rect_vid,
                                                          rgba_surface,
                                                          &src_rect_vid, NULL, NULL,
                                                          rotate_flags[vo_rotate & 3]);
        CHECK_ST_WARNING("Error when calling vdp_output_surface_render_output_surface")
        return;
    }
    if (vid_surface_num < 0)
        return;

    if (deint < 2 || deint_surfaces[0] == VDP_INVALID_HANDLE)
        push_deint_surface(surface_render[vid_surface_num]);

    for (i = 0; i <= !!(deint > 1); i++) {
        int field = VDP_VIDEO_MIXER_PICTURE_STRUCTURE_FRAME;
        VdpOutputSurface output_surface;
        if (i) {
            draw_eosd();
            draw_osd();
            flip_page();
        }
        if (deint)
            field = (top_field_first == i) ^ (deint > 1) ?
                    VDP_VIDEO_MIXER_PICTURE_STRUCTURE_BOTTOM_FIELD:
                    VDP_VIDEO_MIXER_PICTURE_STRUCTURE_TOP_FIELD;
        output_surface = output_surfaces[surface_num];
        vdp_st = vdp_presentation_queue_block_until_surface_idle(vdp_flip_queue,
                                                                 output_surface,
                                                                 &dummy);
        CHECK_ST_WARNING("Error when calling vdp_presentation_queue_block_until_surface_idle")

        vdp_st = vdp_video_mixer_render(video_mixer, VDP_INVALID_HANDLE, 0,
                                        field, 2, deint_surfaces + 1,
                                        deint_surfaces[0],
                                        1, &surface_render[vid_surface_num],
                                        &src_rect_vid,
                                        output_surface,
                                        NULL, &out_rect_vid, 0, NULL);
        CHECK_ST_WARNING("Error when calling vdp_video_mixer_render")
        push_deint_surface(surface_render[vid_surface_num]);
    }
}

static void resize(void)
{
    VdpStatus vdp_st;
    int i;
    struct vo_rect src_rect;
    struct vo_rect dst_rect;
    struct vo_rect borders;
    calc_src_dst_rects(vid_width, vid_height, &src_rect, &dst_rect, &borders, NULL);
    out_rect_vid.x0 = dst_rect.left;
    out_rect_vid.x1 = dst_rect.right;
    out_rect_vid.y0 = dst_rect.top;
    out_rect_vid.y1 = dst_rect.bottom;
    src_rect_vid.x0 = src_rect.left;
    src_rect_vid.x1 = src_rect.right;
    src_rect_vid.y0 = flip ? src_rect.bottom : src_rect.top;
    src_rect_vid.y1 = flip ? src_rect.top    : src_rect.bottom;
    border_l        = borders.left;
    border_t        = borders.top;
    border_r        = borders.right;
    border_b        = borders.bottom;
#ifdef CONFIG_FREETYPE
    // adjust font size to display size
    force_load_font = 1;
#endif
    vo_osd_changed(OSDTYPE_OSD);

    if (output_surface_width < vo_dwidth || output_surface_height < vo_dheight) {
        if (output_surface_width < vo_dwidth) {
            output_surface_width += output_surface_width >> 1;
            output_surface_width = FFMAX(output_surface_width, vo_dwidth);
        }
        if (output_surface_height < vo_dheight) {
            output_surface_height += output_surface_height >> 1;
            output_surface_height = FFMAX(output_surface_height, vo_dheight);
        }
        // Creation of output_surfaces
        for (i = 0; i <= NUM_OUTPUT_SURFACES; i++) {
            if (output_surfaces[i] != VDP_INVALID_HANDLE)
                vdp_output_surface_destroy(output_surfaces[i]);
            vdp_st = vdp_output_surface_create(vdp_device, VDP_RGBA_FORMAT_B8G8R8A8,
                                               output_surface_width, output_surface_height,
                                               &output_surfaces[i]);
            CHECK_ST_WARNING("Error when calling vdp_output_surface_create")
            mp_msg(MSGT_VO, MSGL_DBG2, "OUT CREATE: %u\n", output_surfaces[i]);
        }
    }
    // clear surfaces
    for (i = 0; i <= NUM_OUTPUT_SURFACES; i++) {
        vdp_output_surface_render_output_surface(output_surfaces[i], NULL,
                                                 VDP_INVALID_HANDLE, NULL,
                                                 NULL, NULL, VDP_OUTPUT_SURFACE_RENDER_ROTATE_0);
    }
    video_to_output_surface();
    if (visible_buf) {
        draw_eosd();
        draw_osd();
        flip_page();
    }
}

static void preemption_callback(VdpDevice device, void *context)
{
    mp_msg(MSGT_VO, MSGL_ERR, "[vdpau] Display preemption detected\n");
    is_preempted = 1;
}

static void mark_vdpau_objects_uninitialized(void);

/* Initialize vdp_get_proc_address, called from preinit() */
static int win_x11_init_vdpau_procs(void)
{
    VdpStatus vdp_st;

    struct vdp_function {
        const int id;
        void *pointer;
    };

    const struct vdp_function *dsc;

    static const struct vdp_function vdp_func[] = {
        {VDP_FUNC_ID_GET_ERROR_STRING,          &vdp_get_error_string},
        {VDP_FUNC_ID_DEVICE_DESTROY,            &vdp_device_destroy},
        {VDP_FUNC_ID_VIDEO_SURFACE_CREATE,      &vdp_video_surface_create},
        {VDP_FUNC_ID_VIDEO_SURFACE_DESTROY,     &vdp_video_surface_destroy},
        {VDP_FUNC_ID_VIDEO_SURFACE_PUT_BITS_Y_CB_CR,
                        &vdp_video_surface_put_bits_y_cb_cr},
        {VDP_FUNC_ID_OUTPUT_SURFACE_PUT_BITS_NATIVE,
                        &vdp_output_surface_put_bits_native},
        {VDP_FUNC_ID_OUTPUT_SURFACE_CREATE,     &vdp_output_surface_create},
        {VDP_FUNC_ID_OUTPUT_SURFACE_DESTROY,    &vdp_output_surface_destroy},
        {VDP_FUNC_ID_VIDEO_MIXER_CREATE, &vdp_video_mixer_create},
        {VDP_FUNC_ID_VIDEO_MIXER_DESTROY,       &vdp_video_mixer_destroy},
        {VDP_FUNC_ID_VIDEO_MIXER_RENDER,        &vdp_video_mixer_render},
        {VDP_FUNC_ID_VIDEO_MIXER_SET_FEATURE_ENABLES,
                        &vdp_video_mixer_set_feature_enables},
        {VDP_FUNC_ID_VIDEO_MIXER_SET_ATTRIBUTE_VALUES,
                        &vdp_video_mixer_set_attribute_values},
        {VDP_FUNC_ID_PRESENTATION_QUEUE_TARGET_DESTROY,
                        &vdp_presentation_queue_target_destroy},
        {VDP_FUNC_ID_PRESENTATION_QUEUE_CREATE, &vdp_presentation_queue_create},
        {VDP_FUNC_ID_PRESENTATION_QUEUE_DESTROY,
                        &vdp_presentation_queue_destroy},
        {VDP_FUNC_ID_PRESENTATION_QUEUE_DISPLAY,
                        &vdp_presentation_queue_display},
        {VDP_FUNC_ID_PRESENTATION_QUEUE_BLOCK_UNTIL_SURFACE_IDLE,
                        &vdp_presentation_queue_block_until_surface_idle},
        {VDP_FUNC_ID_PRESENTATION_QUEUE_TARGET_CREATE_X11,
                        &vdp_presentation_queue_target_create_x11},
        {VDP_FUNC_ID_PRESENTATION_QUEUE_SET_BACKGROUND_COLOR,
                        &vdp_presentation_queue_set_background_color},
        {VDP_FUNC_ID_OUTPUT_SURFACE_RENDER_OUTPUT_SURFACE,
                        &vdp_output_surface_render_output_surface},
        {VDP_FUNC_ID_OUTPUT_SURFACE_PUT_BITS_INDEXED,
                        &vdp_output_surface_put_bits_indexed},
        {VDP_FUNC_ID_DECODER_CREATE,            &vdp_decoder_create},
        {VDP_FUNC_ID_DECODER_RENDER,            &vdp_decoder_render},
        {VDP_FUNC_ID_DECODER_DESTROY,           &vdp_decoder_destroy},
        {VDP_FUNC_ID_BITMAP_SURFACE_CREATE,     &vdp_bitmap_surface_create},
        {VDP_FUNC_ID_BITMAP_SURFACE_DESTROY,    &vdp_bitmap_surface_destroy},
        {VDP_FUNC_ID_BITMAP_SURFACE_PUT_BITS_NATIVE,
                        &vdp_bitmap_surface_putbits_native},
        {VDP_FUNC_ID_OUTPUT_SURFACE_RENDER_BITMAP_SURFACE,
                        &vdp_output_surface_render_bitmap_surface},
        {VDP_FUNC_ID_GENERATE_CSC_MATRIX,       &vdp_generate_csc_matrix},
        {VDP_FUNC_ID_PREEMPTION_CALLBACK_REGISTER,
                        &vdp_preemption_callback_register},
        {0, NULL}
    };

    mark_vdpau_objects_uninitialized();

    vdp_st = vdp_device_create_x11(mDisplay, mScreen,
                               &vdp_device, &vdp_get_proc_address);
    if (vdp_st != VDP_STATUS_OK) {
        mp_msg(MSGT_VO, MSGL_ERR, "[vdpau] Error when calling vdp_device_create_x11: %i\n", vdp_st);
        return -1;
    }

    vdp_get_error_string = NULL;
    for (dsc = vdp_func; dsc->pointer; dsc++) {
        vdp_st = vdp_get_proc_address(vdp_device, dsc->id, dsc->pointer);
        if (vdp_st != VDP_STATUS_OK) {
            mp_msg(MSGT_VO, MSGL_ERR, "[vdpau] Error when calling vdp_get_proc_address(function id %d): %s\n",
                   dsc->id, vdp_get_error_string ? vdp_get_error_string(vdp_st) : "?");
            return -1;
        }
    }
    vdp_st = vdp_preemption_callback_register(vdp_device,
                                              preemption_callback, NULL);
    CHECK_ST_ERROR("Error when calling vdp_preemption_callback_register")

    return 0;
}

/* Initialize vdpau_flip_queue, called from config() */
static int win_x11_init_vdpau_flip_queue(void)
{
    VdpStatus vdp_st;
    // {0, 0, 0, 0} makes the video shine through any black window on top
    VdpColor vdp_bg = {0.01, 0.02, 0.03, 0};

    vdp_st = vdp_presentation_queue_target_create_x11(vdp_device, vo_window,
                                                      &vdp_flip_target);
    CHECK_ST_ERROR("Error when calling vdp_presentation_queue_target_create_x11")

    vdp_st = vdp_presentation_queue_create(vdp_device, vdp_flip_target,
                                           &vdp_flip_queue);
    CHECK_ST_ERROR("Error when calling vdp_presentation_queue_create")

    vdp_st = vdp_presentation_queue_set_background_color(vdp_flip_queue, &vdp_bg);
    CHECK_ST_ERROR("Error when calling vdp_presentation_queue_set_background_color")
    return 0;
}

static int update_csc_matrix(void)
{
    VdpStatus vdp_st;
    VdpCSCMatrix matrix;
    static const VdpVideoMixerAttribute attributes[] = {VDP_VIDEO_MIXER_ATTRIBUTE_CSC_MATRIX};
    const void *attribute_values[] = {&matrix};
    static const VdpColorStandard vdp_colors[] = {0, VDP_COLOR_STANDARD_ITUR_BT_601, VDP_COLOR_STANDARD_ITUR_BT_709, VDP_COLOR_STANDARD_SMPTE_240M};
    static const char * const vdp_names[] = {NULL, "BT.601", "BT.709", "SMPTE-240M"};
    int csp = colorspace;

    if (!csp)
        csp = vid_width >= 1280 || vid_height > 576 ? 2 : 1;

    mp_msg(MSGT_VO, MSGL_V, "[vdpau] Updating CSC matrix for %s\n",
           vdp_names[csp]);

    vdp_st = vdp_generate_csc_matrix(&procamp, vdp_colors[csp], &matrix);
    CHECK_ST_WARNING("Error when generating CSC matrix")

    vdp_st = vdp_video_mixer_set_attribute_values(video_mixer, 1, attributes,
                                                  attribute_values);
    CHECK_ST_WARNING("Error when setting CSC matrix")
    return VO_TRUE;
}

static int create_vdp_mixer(VdpChromaType vdp_chroma_type)
{
#define VDP_NUM_MIXER_PARAMETER 3
#define MAX_NUM_FEATURES 6
    int i;
    VdpStatus vdp_st;
    int feature_count = 0;
    VdpVideoMixerFeature features[MAX_NUM_FEATURES];
    VdpBool feature_enables[MAX_NUM_FEATURES];
    static const VdpVideoMixerAttribute denoise_attrib[] = {VDP_VIDEO_MIXER_ATTRIBUTE_NOISE_REDUCTION_LEVEL};
    const void * const denoise_value[] = {&denoise};
    static const VdpVideoMixerAttribute sharpen_attrib[] = {VDP_VIDEO_MIXER_ATTRIBUTE_SHARPNESS_LEVEL};
    const void * const sharpen_value[] = {&sharpen};
    static const VdpVideoMixerAttribute skip_chroma_attrib[] = {VDP_VIDEO_MIXER_ATTRIBUTE_SKIP_CHROMA_DEINTERLACE};
    const uint8_t skip_chroma_value = 1;
    const void * const skip_chroma_value_ptr[] = {&skip_chroma_value};
    static const VdpVideoMixerParameter parameters[VDP_NUM_MIXER_PARAMETER] = {
        VDP_VIDEO_MIXER_PARAMETER_VIDEO_SURFACE_WIDTH,
        VDP_VIDEO_MIXER_PARAMETER_VIDEO_SURFACE_HEIGHT,
        VDP_VIDEO_MIXER_PARAMETER_CHROMA_TYPE
    };
    const void *const parameter_values[VDP_NUM_MIXER_PARAMETER] = {
        &vid_width,
        &vid_height,
        &vdp_chroma_type
    };
    features[feature_count++] = VDP_VIDEO_MIXER_FEATURE_DEINTERLACE_TEMPORAL;
    if (deint == 4)
        features[feature_count++] = VDP_VIDEO_MIXER_FEATURE_DEINTERLACE_TEMPORAL_SPATIAL;
    if (pullup)
        features[feature_count++] = VDP_VIDEO_MIXER_FEATURE_INVERSE_TELECINE;
    if (denoise)
        features[feature_count++] = VDP_VIDEO_MIXER_FEATURE_NOISE_REDUCTION;
    if (sharpen)
        features[feature_count++] = VDP_VIDEO_MIXER_FEATURE_SHARPNESS;
    if (hqscaling)
        features[feature_count++] = VDP_VIDEO_MIXER_FEATURE_HIGH_QUALITY_SCALING_L1 + (hqscaling - 1);

    vdp_st = vdp_video_mixer_create(vdp_device, feature_count, features,
                                    VDP_NUM_MIXER_PARAMETER,
                                    parameters, parameter_values,
                                    &video_mixer);
    CHECK_ST_ERROR("Error when calling vdp_video_mixer_create")

    for (i = 0; i < feature_count; i++)
        feature_enables[i] = VDP_TRUE;
    if (deint < 3)
        feature_enables[0] = VDP_FALSE;
    if (feature_count)
        vdp_video_mixer_set_feature_enables(video_mixer, feature_count, features, feature_enables);
    if (denoise)
        vdp_video_mixer_set_attribute_values(video_mixer, 1, denoise_attrib, denoise_value);
    if (sharpen)
        vdp_video_mixer_set_attribute_values(video_mixer, 1, sharpen_attrib, sharpen_value);
    if (!chroma_deint)
        vdp_video_mixer_set_attribute_values(video_mixer, 1, skip_chroma_attrib, skip_chroma_value_ptr);

    update_csc_matrix();
    return 0;
}

static void mark_invalid(void)
{
    int i;

    decoder = VDP_INVALID_HANDLE;
    video_mixer = VDP_INVALID_HANDLE;

    rgba_surface = VDP_INVALID_HANDLE;

    for (i = 0; i < 3; i++)
        deint_surfaces[i] = VDP_INVALID_HANDLE;

    for (i = 0; i < 2; i++)
        if (deint_mpi[i]) {
            deint_mpi[i]->usage_count--;
            deint_mpi[i] = NULL;
        }
}

// Free everything specific to a certain video file
static void free_video_specific(void)
{
    int i;
    VdpStatus vdp_st;

    if (decoder != VDP_INVALID_HANDLE)
        vdp_decoder_destroy(decoder);
    decoder_max_refs = -1;

    if (rgba_surface != VDP_INVALID_HANDLE) {
        vdp_st = vdp_output_surface_destroy(rgba_surface);
        CHECK_ST_WARNING("Error destroying RGBA surface")
    }

    for (i = 0; i < MAX_VIDEO_SURFACES; i++) {
        if (surface_render[i] != VDP_INVALID_HANDLE) {
          vdp_st = vdp_video_surface_destroy(surface_render[i]);
          CHECK_ST_WARNING("Error when calling vdp_video_surface_destroy")
        }
        surface_render[i] = VDP_INVALID_HANDLE;
    }

    if (video_mixer != VDP_INVALID_HANDLE) {
        vdp_st = vdp_video_mixer_destroy(video_mixer);
        CHECK_ST_WARNING("Error when calling vdp_video_mixer_destroy")
    }
    mark_invalid();
}

static int create_vdp_decoder(uint32_t format, uint32_t width, uint32_t height,
                              int max_refs)
{
    VdpStatus vdp_st;
    VdpDecoderProfile vdp_decoder_profile;
    if (decoder != VDP_INVALID_HANDLE)
        vdp_decoder_destroy(decoder);
    switch (format) {
    case IMGFMT_VDPAU_MPEG1:
        vdp_decoder_profile = VDP_DECODER_PROFILE_MPEG1;
        break;
    case IMGFMT_VDPAU_MPEG2:
        vdp_decoder_profile = VDP_DECODER_PROFILE_MPEG2_MAIN;
        break;
    case IMGFMT_VDPAU_H264:
        vdp_decoder_profile = VDP_DECODER_PROFILE_H264_HIGH;
        mp_msg(MSGT_VO, MSGL_V, "[vdpau] Creating H264 hardware decoder for %d reference frames.\n", max_refs);
        break;
    case IMGFMT_VDPAU_WMV3:
        vdp_decoder_profile = VDP_DECODER_PROFILE_VC1_MAIN;
        break;
    case IMGFMT_VDPAU_VC1:
        vdp_decoder_profile = VDP_DECODER_PROFILE_VC1_ADVANCED;
        break;
    case IMGFMT_VDPAU_MPEG4:
        vdp_decoder_profile = VDP_DECODER_PROFILE_MPEG4_PART2_ASP;
        break;
#ifdef VDP_DECODER_PROFILE_HEVC_MAIN
    case IMGFMT_VDPAU_HEVC:
        vdp_decoder_profile = VDP_DECODER_PROFILE_HEVC_MAIN;
        mp_msg(MSGT_VO, MSGL_V, "[vdpau] Creating HEVC hardware decoder for %d reference frames.\n", max_refs);
        break;
#endif
    default:
        goto err_out;
    }
    vdp_st = vdp_decoder_create(vdp_device, vdp_decoder_profile,
                                width, height, max_refs, &decoder);
    CHECK_ST_WARNING("Failed creating VDPAU decoder");
    if (vdp_st != VDP_STATUS_OK) {
err_out:
        decoder = VDP_INVALID_HANDLE;
        decoder_max_refs = 0;
        return 0;
    }
    decoder_max_refs = max_refs;
    return 1;
}

static void mark_vdpau_objects_uninitialized(void)
{
    int i;

    for (i = 0; i < MAX_VIDEO_SURFACES; i++)
        surface_render[i] = VDP_INVALID_HANDLE;
    vdp_flip_queue  = VDP_INVALID_HANDLE;
    vdp_flip_target = VDP_INVALID_HANDLE;
    for (i = 0; i <= NUM_OUTPUT_SURFACES; i++)
        output_surfaces[i] = VDP_INVALID_HANDLE;
    vdp_device = VDP_INVALID_HANDLE;
    for (i = 0; i < eosd_surface_count; i++)
        eosd_surfaces[i].surface = VDP_INVALID_HANDLE;
    output_surface_width = output_surface_height = -1;
    eosd_render_count = 0;
    visible_buf = 0;
    mark_invalid();
}

static int handle_preemption(void)
{
    if (!is_preempted)
        return 0;
    is_preempted = 0;
    mp_msg(MSGT_VO, MSGL_INFO, "[vdpau] Attempting to recover from preemption.\n");
    if (win_x11_init_vdpau_procs() < 0 ||
        win_x11_init_vdpau_flip_queue() < 0 ||
        create_vdp_mixer(vdp_chroma_type) < 0) {
        mp_msg(MSGT_VO, MSGL_ERR, "[vdpau] Recovering from preemption failed\n");
        is_preempted = 1;
        return -1;
    }
    resize();
    mp_msg(MSGT_VO, MSGL_INFO, "[vdpau] Recovered from display preemption.\n");
    return 1;
}

/*
 * connect to X server, create and map window, initialize all
 * VDPAU objects, create different surfaces etc.
 */
static int config(uint32_t width, uint32_t height, uint32_t d_width,
                  uint32_t d_height, uint32_t flags, char *title,
                  uint32_t format)
{
    VdpStatus vdp_st;
    XVisualInfo vinfo;
    XSetWindowAttributes xswa;
    XWindowAttributes attribs;
    unsigned long xswamask;
    int depth;

#ifdef CONFIG_XF86VM
    int vm = flags & VOFLAG_MODESWITCHING;
#endif
    flip = flags & VOFLAG_FLIPPING;

    if (IMGFMT_IS_VDPAU(image_format) && decoder == VDP_INVALID_HANDLE) {
        mp_msg(MSGT_VO, MSGL_WARN, "[vdpau] Unexpected reinit, query_format called too often?\n");
        // force a full reinit, otherwise we might run into issues with
        // some implementations
        image_format = 0;
    }
    if (image_format == format &&
        vid_width    == width  &&
        vid_height   == height) {
        // at most aspect change, avoid reinit
        vo_x11_create_vo_window(&vinfo, vo_dx, vo_dy, d_width, d_height,
                                flags, CopyFromParent, "vdpau", title);
        resize();
        return 0;
    }
    image_format = format;
    vid_width    = width;
    vid_height   = height;
    free_video_specific();
    if (IMGFMT_IS_VDPAU(image_format)
        && !create_vdp_decoder(image_format, vid_width, vid_height, 2))
        return -1;

    visible_buf = 0;

    {
#ifdef CONFIG_XF86VM
        if (vm)
            vo_vm_switch();
#endif
        XGetWindowAttributes(mDisplay, DefaultRootWindow(mDisplay), &attribs);
        depth = attribs.depth;
        if (depth != 15 && depth != 16 && depth != 24 && depth != 32)
            depth = 24;
        XMatchVisualInfo(mDisplay, mScreen, depth, TrueColor, &vinfo);

        xswa.background_pixel = 0;
        xswa.border_pixel     = 0;
        /* Do not use CWBackPixel: It leads to VDPAU errors after
           aspect ratio changes. */
        xswamask = CWBorderPixel;

        vo_x11_create_vo_window(&vinfo, vo_dx, vo_dy, d_width, d_height,
                                flags, CopyFromParent, "vdpau", title);
        XChangeWindowAttributes(mDisplay, vo_window, xswamask, &xswa);

#ifdef CONFIG_XF86VM
        if (vm) {
            /* Grab the mouse pointer in our window */
            if (vo_grabpointer)
                XGrabPointer(mDisplay, vo_window, True, 0,
                             GrabModeAsync, GrabModeAsync,
                             vo_window, None, CurrentTime);
            XSetInputFocus(mDisplay, vo_window, RevertToNone, CurrentTime);
        }
#endif
    }

    if ((flags & VOFLAG_FULLSCREEN) && WinID <= 0)
        vo_fs = 1;

    /* -----VDPAU related code here -------- */
    if (vdp_flip_queue == VDP_INVALID_HANDLE && win_x11_init_vdpau_flip_queue())
        return -1;

    vdp_chroma_type = VDP_CHROMA_TYPE_420;
    switch (image_format) {
    case IMGFMT_YV12:
    case IMGFMT_I420:
    case IMGFMT_IYUV:
        vdp_pixel_format = VDP_YCBCR_FORMAT_YV12;
        break;
    case IMGFMT_NV12:
        vdp_pixel_format = VDP_YCBCR_FORMAT_NV12;
        break;
    case IMGFMT_YUY2:
        vdp_pixel_format = VDP_YCBCR_FORMAT_YUYV;
        vdp_chroma_type  = VDP_CHROMA_TYPE_422;
        break;
    case IMGFMT_UYVY:
        vdp_pixel_format = VDP_YCBCR_FORMAT_UYVY;
        vdp_chroma_type  = VDP_CHROMA_TYPE_422;
        break;
    case IMGFMT_BGRA:
        vdp_st = vdp_output_surface_create(vdp_device, VDP_RGBA_FORMAT_B8G8R8A8,
                                           vid_width, vid_height,
                                           &rgba_surface);
        CHECK_ST_ERROR("Error creating RGBA surface")
        break;
    }
    if (create_vdp_mixer(vdp_chroma_type))
        return -1;

    surface_num     =  0;
    vid_surface_num = -1;
    resize();

    return 0;
}

static void check_events(void)
{
    int e = vo_x11_check_events(mDisplay);

    if (handle_preemption() < 0)
        return;

    if (e & VO_EVENT_RESIZE)
        resize();
    else if (e & VO_EVENT_EXPOSE) {
        /* did we already draw a buffer */
        if (visible_buf) {
            /* redraw the last visible buffer */
            VdpStatus vdp_st;
            vdp_st = vdp_presentation_queue_display(vdp_flip_queue,
                                                    output_surfaces[(surface_num + NUM_OUTPUT_SURFACES - 1) % NUM_OUTPUT_SURFACES],
                                                    vo_dwidth, vo_dheight,
                                                    0);
            CHECK_ST_WARNING("Error when calling vdp_presentation_queue_display")
        }
    }
}

#if HAVE_EMMINTRIN_H
#include <emmintrin.h>

#include "cpudetect.h"

ATTR_TARGET_SSE2
static void copyosd_SSE2(int w, int h, unsigned char *src, unsigned char *srca, int stride)
{
    unsigned char *dst = index_data;
    __m128i zero = _mm_setzero_si128();
    while (h--) {
        int i;
        for (i = 0; i < w - 15; i += 16)
        {
            __m128i mmsrc = _mm_loadu_si128((const __m128i *)(src + i));
            __m128i mmsrca = _mm_loadu_si128((const __m128i *)(srca + i));
            mmsrca = _mm_sub_epi8(zero, mmsrca);
            _mm_storeu_si128((__m128i *)(dst + 0), _mm_unpacklo_epi8(mmsrc, mmsrca));
            _mm_storeu_si128((__m128i *)(dst + 16), _mm_unpackhi_epi8(mmsrc, mmsrca));
            dst += 32;
        }
        for (; i < w; i++)
        {
            *dst++ = src  [i];
            *dst++ = -srca[i];
        }
        src += stride;
        srca += stride;
    }
}
#endif

static void draw_osd_I8A8(int x0,int y0, int w,int h, unsigned char *src,
                          unsigned char *srca, int stride)
{
    VdpOutputSurface output_surface = output_surfaces[surface_num];
    VdpStatus vdp_st;
    int i, j;
    int pitch;
    int index_data_size_required;
    VdpRect output_indexed_rect_vid;
    VdpOutputSurfaceRenderBlendState blend_state;

    if (!w || !h)
        return;

    index_data_size_required = 2*w*h;
    if (index_data_size < index_data_size_required) {
        index_data      = realloc(index_data, index_data_size_required);
        index_data_size = index_data_size_required;
    }

    // index_data creation, component order - I, A, I, A, .....
#if HAVE_EMMINTRIN_H
    if (gCpuCaps.hasSSE2) copyosd_SSE2(w, h, src, srca, stride);
    else
#endif
    {
        unsigned char *dst = index_data;
        for (i = 0; i < h; i++)
            for (j = 0; j < w; j++) {
                *dst++ = src [i*stride + j];
                *dst++ = -srca[i*stride + j];
            }
    }

    output_indexed_rect_vid.x0 = x0;
    output_indexed_rect_vid.y0 = y0;
    output_indexed_rect_vid.x1 = x0 + w;
    output_indexed_rect_vid.y1 = y0 + h;

    pitch = w*2;

    // write source_data to osd_surface.
    vdp_st = vdp_output_surface_put_bits_indexed(osd_surface,
                                                 VDP_INDEXED_FORMAT_I8A8,
                                                 (const void *const*)&index_data,
                                                 &pitch,
                                                 &output_indexed_rect_vid,
                                                 VDP_COLOR_TABLE_FORMAT_B8G8R8X8,
                                                 (void *)palette);
    CHECK_ST_WARNING("Error when calling vdp_output_surface_put_bits_indexed")

    blend_state.struct_version                 = VDP_OUTPUT_SURFACE_RENDER_BLEND_STATE_VERSION;
    blend_state.blend_factor_source_color      = VDP_OUTPUT_SURFACE_RENDER_BLEND_FACTOR_ONE;
    blend_state.blend_factor_source_alpha      = VDP_OUTPUT_SURFACE_RENDER_BLEND_FACTOR_ZERO;
    blend_state.blend_factor_destination_color = VDP_OUTPUT_SURFACE_RENDER_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    blend_state.blend_factor_destination_alpha = VDP_OUTPUT_SURFACE_RENDER_BLEND_FACTOR_ZERO;
    blend_state.blend_equation_color           = VDP_OUTPUT_SURFACE_RENDER_BLEND_EQUATION_ADD;
    blend_state.blend_equation_alpha           = VDP_OUTPUT_SURFACE_RENDER_BLEND_EQUATION_ADD;

    vdp_st = vdp_output_surface_render_output_surface(output_surface,
                                                      &output_indexed_rect_vid,
                                                      osd_surface,
                                                      &output_indexed_rect_vid,
                                                      NULL,
                                                      &blend_state,
                                                      rotate_flags[vo_rotate & 3]);
    CHECK_ST_WARNING("Error when calling vdp_output_surface_render_output_surface")
}

static void draw_eosd(void)
{
    VdpStatus vdp_st;
    VdpOutputSurface output_surface = output_surfaces[surface_num];
    VdpOutputSurfaceRenderBlendState blend_state;
    int i;

    blend_state.struct_version                 = VDP_OUTPUT_SURFACE_RENDER_BLEND_STATE_VERSION;
    blend_state.blend_factor_source_color      = VDP_OUTPUT_SURFACE_RENDER_BLEND_FACTOR_SRC_ALPHA;
    blend_state.blend_factor_source_alpha      = VDP_OUTPUT_SURFACE_RENDER_BLEND_FACTOR_ZERO;
    blend_state.blend_factor_destination_color = VDP_OUTPUT_SURFACE_RENDER_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    blend_state.blend_factor_destination_alpha = VDP_OUTPUT_SURFACE_RENDER_BLEND_FACTOR_ZERO;
    blend_state.blend_equation_color           = VDP_OUTPUT_SURFACE_RENDER_BLEND_EQUATION_ADD;
    blend_state.blend_equation_alpha           = VDP_OUTPUT_SURFACE_RENDER_BLEND_EQUATION_ADD;

    for (i = 0; i < eosd_render_count; i++) {
        vdp_st = vdp_output_surface_render_bitmap_surface(
            output_surface, &eosd_targets[i].dest,
            eosd_targets[i].surface, &eosd_targets[i].source,
            &eosd_targets[i].color, &blend_state,
            rotate_flags[vo_rotate & 3]);
        CHECK_ST_WARNING("EOSD: Error when rendering")
    }
}

static void generate_eosd(struct mp_eosd_image_list *imgs)
{
    VdpStatus vdp_st;
    VdpRect destRect;
    int j, found;
    struct mp_eosd_image *img = eosd_image_first(imgs);
    struct mp_eosd_image *i;

    // Nothing changed, no need to redraw
    if (imgs->changed == 0)
        return;
    eosd_render_count = 0;
    // There's nothing to render!
    if (!img)
        return;

    if (imgs->changed == 1)
        goto eosd_skip_upload;

    for (j = 0; j < eosd_surface_count; j++)
        eosd_surfaces[j].in_use = 0;

    for (i = img; i; i = eosd_image_next(imgs)) {
        // Try to reuse a suitable surface
        found = -1;
        for (j = 0; j < eosd_surface_count; j++) {
            if (eosd_surfaces[j].surface != VDP_INVALID_HANDLE && !eosd_surfaces[j].in_use &&
                eosd_surfaces[j].w >= i->w && eosd_surfaces[j].h >= i->h) {
                found = j;
                break;
            }
        }
        // None found, allocate a new surface
        if (found < 0) {
            for (j = 0; j < eosd_surface_count; j++) {
                if (!eosd_surfaces[j].in_use) {
                    if (eosd_surfaces[j].surface != VDP_INVALID_HANDLE)
                        vdp_bitmap_surface_destroy(eosd_surfaces[j].surface);
                    found = j;
                    break;
                }
            }
            // Allocate new space for surface/target arrays
            if (found < 0) {
                j = found = eosd_surface_count;
                eosd_surface_count = eosd_surface_count ? eosd_surface_count*2 : EOSD_SURFACES_INITIAL;
                eosd_surfaces = realloc(eosd_surfaces, eosd_surface_count * sizeof(*eosd_surfaces));
                eosd_targets  = realloc(eosd_targets,  eosd_surface_count * sizeof(*eosd_targets));
                for (j = found; j < eosd_surface_count; j++) {
                    eosd_surfaces[j].surface = VDP_INVALID_HANDLE;
                    eosd_surfaces[j].in_use = 0;
                }
            }
            vdp_st = vdp_bitmap_surface_create(vdp_device, VDP_RGBA_FORMAT_A8,
                i->w, i->h, VDP_TRUE, &eosd_surfaces[found].surface);
            CHECK_ST_WARNING("EOSD: error when creating surface")
            eosd_surfaces[found].w = i->w;
            eosd_surfaces[found].h = i->h;
        }
        eosd_surfaces[found].in_use = 1;
        eosd_targets[eosd_render_count].surface = eosd_surfaces[found].surface;
        destRect.x0 = 0;
        destRect.y0 = 0;
        destRect.x1 = i->w;
        destRect.y1 = i->h;
        vdp_st = vdp_bitmap_surface_putbits_native(eosd_targets[eosd_render_count].surface,
            (const void *) &i->bitmap, &i->stride, &destRect);
        CHECK_ST_WARNING("EOSD: putbits failed")
        eosd_render_count++;
    }

eosd_skip_upload:
    eosd_render_count = 0;
    for (i = eosd_image_first(imgs); i; i = eosd_image_next(imgs)) {
        // Render dest, color, etc.
        eosd_targets[eosd_render_count].color.alpha = 1.0 - ((i->color >>  0) & 0xff) / 255.0;
        eosd_targets[eosd_render_count].color.blue  =       ((i->color >>  8) & 0xff) / 255.0;
        eosd_targets[eosd_render_count].color.green =       ((i->color >> 16) & 0xff) / 255.0;
        eosd_targets[eosd_render_count].color.red   =       ((i->color >> 24) & 0xff) / 255.0;
        eosd_targets[eosd_render_count].dest.x0     =        i->dst_x;
        eosd_targets[eosd_render_count].dest.y0     =        i->dst_y;
        eosd_targets[eosd_render_count].dest.x1     = i->w + i->dst_x;
        eosd_targets[eosd_render_count].dest.y1     = i->h + i->dst_y;
        eosd_targets[eosd_render_count].source.x0   = 0;
        eosd_targets[eosd_render_count].source.y0   = 0;
        eosd_targets[eosd_render_count].source.x1   = i->w;
        eosd_targets[eosd_render_count].source.y1   = i->h;
        eosd_render_count++;
    }
}

static void draw_osd(void)
{
    mp_msg(MSGT_VO, MSGL_DBG2, "DRAW_OSD\n");

    if (handle_preemption() < 0)
        return;

    vo_draw_text_ext(vo_dwidth, vo_dheight, border_l, border_t, border_r, border_b,
                     vid_width, vid_height, draw_osd_I8A8);
}

static void flip_page(void)
{
    VdpStatus vdp_st;
    mp_msg(MSGT_VO, MSGL_DBG2, "\nFLIP_PAGE VID:%u -> OUT:%u\n",
           surface_render[vid_surface_num], output_surfaces[surface_num]);

    if (handle_preemption() < 0)
        return;

    vdp_st = vdp_presentation_queue_display(vdp_flip_queue, output_surfaces[surface_num],
                                            vo_dwidth, vo_dheight,
                                            0);
    CHECK_ST_WARNING("Error when calling vdp_presentation_queue_display")

    surface_num = (surface_num + 1) % NUM_OUTPUT_SURFACES;
    visible_buf = 1;
}

static int draw_slice(uint8_t *image[], int stride[], int w, int h,
                      int x, int y)
{
    VdpStatus vdp_st;
    struct vdpau_frame_data *rndr = (struct vdpau_frame_data *)image[0];
    int max_refs;
    switch (image_format) {
    case IMGFMT_VDPAU_H264:
        max_refs = ((VdpPictureInfoH264 *)rndr->info)->num_ref_frames;
        break;
    case IMGFMT_VDPAU_HEVC:
        max_refs = 16;
        break;
    default:
        max_refs = 2;
    }

    if (handle_preemption() < 0)
        return VO_TRUE;

    if (!IMGFMT_IS_VDPAU(image_format))
        return VO_FALSE;
    if ((decoder == VDP_INVALID_HANDLE || decoder_max_refs < max_refs)
        && !create_vdp_decoder(image_format, vid_width, vid_height, max_refs))
        return VO_FALSE;

    vdp_st = vdp_decoder_render(decoder, rndr->surface, rndr->info, rndr->bitstream_buffers_used, rndr->bitstream_buffers);
    CHECK_ST_WARNING("Failed VDPAU decoder rendering");
    return VO_TRUE;
}

static int get_surface_idx(VdpVideoSurface surface)
{
    int i;
    for (i = 0; i < MAX_VIDEO_SURFACES; i++) {
      if (surface == surface_render[i]) {
          return i;
      }
    }
    mp_msg(MSGT_VO, MSGL_FATAL, "Unknown VdpVideoSurface %u!\n", surface);
    return -1;
}

static VdpVideoSurface get_surface(int number)
{
    if (number >= MAX_VIDEO_SURFACES)
        return VDP_INVALID_HANDLE;
    if (surface_render[number] == VDP_INVALID_HANDLE) {
        VdpStatus vdp_st;
        vdp_st = vdp_video_surface_create(vdp_device, vdp_chroma_type,
                                          vid_width, vid_height,
                                          &surface_render[number]);
        CHECK_ST_WARNING("Error when calling vdp_video_surface_create")
        if (vdp_st != VDP_STATUS_OK)
            return VDP_INVALID_HANDLE;
    }
    mp_msg(MSGT_VO, MSGL_DBG2, "VID CREATE: %u\n", surface_render[number]);
    return surface_render[number];
}

static uint32_t draw_image(mp_image_t *mpi)
{
    if (IMGFMT_IS_VDPAU(image_format)) {
        VdpVideoSurface surface = (VdpVideoSurface)mpi->priv;
        vid_surface_num = get_surface_idx(surface);
        mpi->usage_count++;
        if (deint_mpi[1])
            deint_mpi[1]->usage_count--;
        deint_mpi[1] = deint_mpi[0];
        deint_mpi[0] = mpi;
    } else if (image_format == IMGFMT_BGRA) {
        VdpStatus vdp_st;
        VdpRect r = {0, 0, vid_width, vid_height};
        vdp_st = vdp_output_surface_put_bits_native(rgba_surface,
                                                    (void const*const*)mpi->planes,
                                                    mpi->stride, &r);
        CHECK_ST_ERROR("Error when calling vdp_output_surface_put_bits_native")
    } else if (!(mpi->flags & MP_IMGFLAG_DRAW_CALLBACK)) {
        VdpStatus vdp_st;
        void *destdata[3] = {mpi->planes[0], mpi->planes[2], mpi->planes[1]};
        VdpVideoSurface surface = get_surface(deint_counter);
        deint_counter = (deint_counter + 1) % 3;
        vid_surface_num = get_surface_idx(surface);
        if (image_format == IMGFMT_NV12)
            destdata[1] = destdata[2];
        vdp_st = vdp_video_surface_put_bits_y_cb_cr(surface,
                                                    vdp_pixel_format,
                                                    (const void *const*)destdata,
                                                    mpi->stride); // pitch
        CHECK_ST_ERROR("Error when calling vdp_video_surface_put_bits_y_cb_cr")
    }
    if (mpi->fields & MP_IMGFIELD_ORDERED)
        top_field_first = !!(mpi->fields & MP_IMGFIELD_TOP_FIRST);
    else
        top_field_first = 1;

    video_to_output_surface();
    return VO_TRUE;
}

static uint32_t get_image(mp_image_t *mpi)
{
    VdpVideoSurface surface;

    // no dr for non-decoding for now
    if (!IMGFMT_IS_VDPAU(image_format))
        return VO_FALSE;
    if (mpi->type != MP_IMGTYPE_NUMBERED)
        return VO_FALSE;

    surface = get_surface(mpi->number);
    if (surface == VDP_INVALID_HANDLE) {
        mp_msg(MSGT_VO, MSGL_ERR, "[vdpau] no surfaces available in get_image\n");
        // TODO: this probably breaks things forever, provide a dummy buffer?
        return VO_FALSE;
    }
    mpi->flags |= MP_IMGFLAG_DIRECT;
    mpi->stride[0] = mpi->stride[1] = mpi->stride[2] = 0;
    mpi->planes[0] = mpi->planes[1] = mpi->planes[2] = NULL;
    // hack to get around a check and to avoid a special-case in vd_ffmpeg.c
    mpi->planes[0] = (void *)(uintptr_t)surface;
    mpi->num_planes = 1;
    mpi->priv = (void *)(uintptr_t)surface;
    return VO_TRUE;
}

static int query_format(uint32_t format)
{
    int default_flags = VFCAP_CSP_SUPPORTED | VFCAP_CSP_SUPPORTED_BY_HW | VFCAP_HWSCALE_UP | VFCAP_HWSCALE_DOWN | VFCAP_OSD | VFCAP_EOSD | VFCAP_EOSD_UNSCALED | VFCAP_FLIP;
    // if we are already using it we obviously support it
    if (format == image_format && decoder != VDP_INVALID_HANDLE)
        return default_flags;
    switch (format) {
    case IMGFMT_BGRA:
        if (force_mixer)
            return 0;
    case IMGFMT_YV12:
    case IMGFMT_I420:
    case IMGFMT_IYUV:
    case IMGFMT_NV12:
    case IMGFMT_YUY2:
    case IMGFMT_UYVY:
        return default_flags | VOCAP_NOSLICES;
    case IMGFMT_VDPAU_MPEG1:
    case IMGFMT_VDPAU_MPEG2:
    case IMGFMT_VDPAU_H264:
    case IMGFMT_VDPAU_WMV3:
    case IMGFMT_VDPAU_VC1:
    case IMGFMT_VDPAU_MPEG4:
    case IMGFMT_VDPAU_HEVC:
        // Note: this will break the current decoder
        // Not all implementations support safely instantiating
        // a second decoder, so this is the "lesser evil"
        if (create_vdp_decoder(format, 48, 48, 2)) {
            vdp_decoder_destroy(decoder);
            decoder = VDP_INVALID_HANDLE;
            decoder_max_refs = -1;
            return default_flags;
        }
    }
    return 0;
}

static void DestroyVdpauObjects(void)
{
    int i;
    VdpStatus vdp_st;

    free_video_specific();

    vdp_st = vdp_presentation_queue_destroy(vdp_flip_queue);
    CHECK_ST_WARNING("Error when calling vdp_presentation_queue_destroy")

    vdp_st = vdp_presentation_queue_target_destroy(vdp_flip_target);
    CHECK_ST_WARNING("Error when calling vdp_presentation_queue_target_destroy")

    for (i = 0; i <= NUM_OUTPUT_SURFACES; i++) {
        vdp_st = vdp_output_surface_destroy(output_surfaces[i]);
        output_surfaces[i] = VDP_INVALID_HANDLE;
        CHECK_ST_WARNING("Error when calling vdp_output_surface_destroy")
    }

    for (i = 0; i<eosd_surface_count; i++) {
        if (eosd_surfaces[i].surface != VDP_INVALID_HANDLE) {
            vdp_st = vdp_bitmap_surface_destroy(eosd_surfaces[i].surface);
            CHECK_ST_WARNING("Error when calling vdp_bitmap_surface_destroy")
        }
        eosd_surfaces[i].surface = VDP_INVALID_HANDLE;
    }

    vdp_st = vdp_device_destroy(vdp_device);
    CHECK_ST_WARNING("Error when calling vdp_device_destroy")
}

static void uninit(void)
{
    if (!vo_config_count)
        return;
    visible_buf = 0;

    /* Destroy all vdpau objects */
    DestroyVdpauObjects();

    free(index_data);
    index_data = NULL;

    free(eosd_surfaces);
    eosd_surfaces = NULL;
    free(eosd_targets);
    eosd_targets = NULL;

#ifdef CONFIG_XF86VM
    vo_vm_close();
#endif
    vo_x11_uninit();
}

static const opt_t subopts[] = {
    {"deint",   OPT_ARG_INT,   &deint,   (opt_test_f)int_non_neg},
    {"chroma-deint", OPT_ARG_BOOL,  &chroma_deint,  NULL},
    {"pullup",  OPT_ARG_BOOL,  &pullup,  NULL},
    {"denoise", OPT_ARG_FLOAT, &denoise, NULL},
    {"sharpen", OPT_ARG_FLOAT, &sharpen, NULL},
    {"colorspace", OPT_ARG_INT, &colorspace, NULL},
    {"force-mixer", OPT_ARG_BOOL, &force_mixer, NULL},
    {"hqscaling", OPT_ARG_INT, &hqscaling, (opt_test_f)int_non_neg},
    {NULL}
};

static const char help_msg[] =
    "\n-vo vdpau command line help:\n"
    "Example: mplayer -vo vdpau:deint=2\n"
    "\nOptions:\n"
    "  deint (all modes > 0 respect -field-dominance)\n"
    "    0: no deinterlacing\n"
    "    1: only show first field\n"
    "    2: bob deinterlacing\n"
    "    3: temporal deinterlacing (resource-hungry)\n"
    "    4: temporal-spatial deinterlacing (very resource-hungry)\n"
    "  chroma-deint\n"
    "    Operate on luma and chroma when using temporal deinterlacing (default)\n"
    "    Use nochroma-deint to speed up temporal deinterlacing\n"
    "  pullup\n"
    "    Try to skip deinterlacing for progressive frames\n"
    "    (needs at least temporal deinterlacing, useful for telecined content)\n"
    "  denoise\n"
    "    Apply denoising, argument is strength from 0.0 to 1.0\n"
    "  sharpen\n"
    "    Apply sharpening or softening, argument is strength from -1.0 to 1.0\n"
    "  colorspace\n"
    "    0: guess based on video resolution\n"
    "    1: ITU-R BT.601 (default)\n"
    "    2: ITU-R BT.709\n"
    "    3: SMPTE-240M\n"
    "  hqscaling\n"
    "    0: default VDPAU scaler\n"
    "    1-9: high quality VDPAU scaler (needs capable hardware)\n"
    "  force-mixer\n"
    "    Use the VDPAU mixer (default)\n"
    "    Use noforce-mixer to allow BGRA output (disables all above options)\n"
    ;

static int preinit(const char *arg)
{
    int i;
    image_format = 0;
    vid_width    = 0;
    vid_height   = 0;

    deint = 0;
    deint_type = 3;
    deint_counter = 0;
    deint_mpi[0] = deint_mpi[1] = NULL;
    chroma_deint = 1;
    pullup  = 0;
    denoise = 0;
    sharpen = 0;
    colorspace = 1;
    force_mixer = 1;
    hqscaling = 0;
    if (subopt_parse(arg, subopts) != 0) {
        mp_msg(MSGT_VO, MSGL_FATAL, help_msg);
        return -1;
    }
    if (deint)
        deint_type = deint;
    if (colorspace < 0 || colorspace > 3) {
        mp_msg(MSGT_VO, MSGL_WARN, "[vdpau] Invalid color space specified, "
               "using BT.601\n");
        colorspace = 1;
    }

    // full grayscale palette.
    for (i = 0; i < PALETTE_SIZE; ++i)
        palette[i] = (i << 16) | (i << 8) | i;
    index_data = NULL;
    index_data_size = 0;

    eosd_surface_count = eosd_render_count = 0;
    eosd_surfaces = NULL;
    eosd_targets  = NULL;

    procamp.struct_version = VDP_PROCAMP_VERSION;
    procamp.brightness = 0.0;
    procamp.contrast   = 1.0;
    procamp.saturation = 1.0;
    procamp.hue        = 0.0;

    if (!vo_init() || win_x11_init_vdpau_procs())
        return -1;

    return 0;
}

static int get_equalizer(const char *name, int *value)
{
    if (!av_strcasecmp(name, "brightness"))
        *value = procamp.brightness * 100;
    else if (!av_strcasecmp(name, "contrast"))
        *value = (procamp.contrast-1.0) * 100;
    else if (!av_strcasecmp(name, "saturation"))
        *value = (procamp.saturation-1.0) * 100;
    else if (!av_strcasecmp(name, "hue"))
        *value = procamp.hue * 100 / M_PI;
    else
        return VO_NOTIMPL;
    return VO_TRUE;
}

static int set_equalizer(const char *name, int value)
{
    if (!av_strcasecmp(name, "brightness"))
        procamp.brightness = value / 100.0;
    else if (!av_strcasecmp(name, "contrast"))
        procamp.contrast = value / 100.0 + 1.0;
    else if (!av_strcasecmp(name, "saturation"))
        procamp.saturation = value / 100.0 + 1.0;
    else if (!av_strcasecmp(name, "hue"))
        procamp.hue = value / 100.0 * M_PI;
    else
        return VO_NOTIMPL;

    return update_csc_matrix();
}

static int control(uint32_t request, void *data)
{
    if (handle_preemption() < 0)
        return VO_FALSE;

    switch (request) {
    case VOCTRL_GET_DEINTERLACE:
        *(int*)data = deint;
        return VO_TRUE;
    case VOCTRL_SET_DEINTERLACE:
        if (image_format == IMGFMT_BGRA)
            return VO_NOTIMPL;
        deint = *(int*)data;
        if (deint)
            deint = deint_type;
        if (deint_type > 2) {
            VdpStatus vdp_st;
            VdpVideoMixerFeature features[1] =
                {deint_type == 3 ?
                 VDP_VIDEO_MIXER_FEATURE_DEINTERLACE_TEMPORAL :
                 VDP_VIDEO_MIXER_FEATURE_DEINTERLACE_TEMPORAL_SPATIAL};
            VdpBool feature_enables[1] = {deint ? VDP_TRUE : VDP_FALSE};
            vdp_st = vdp_video_mixer_set_feature_enables(video_mixer, 1,
                                                         features,
                                                         feature_enables);
            CHECK_ST_WARNING("Error changing deinterlacing settings")
        }
        return VO_TRUE;
    case VOCTRL_QUERY_FORMAT:
        return query_format(*(uint32_t *)data);
    case VOCTRL_GET_IMAGE:
        return get_image(data);
    case VOCTRL_DRAW_IMAGE:
        return draw_image(data);
    case VOCTRL_GUISUPPORT:
        return VO_TRUE;
    case VOCTRL_BORDER:
        vo_x11_border();
        resize();
        return VO_TRUE;
    case VOCTRL_FULLSCREEN:
        vo_x11_fullscreen();
        resize();
        return VO_TRUE;
    case VOCTRL_GET_PANSCAN:
        return VO_TRUE;
    case VOCTRL_SET_PANSCAN:
        resize();
        return VO_TRUE;
    case VOCTRL_SET_EQUALIZER: {
        vf_equalizer_t *eq=data;
        if (image_format == IMGFMT_BGRA)
            return VO_NOTIMPL;

        return set_equalizer(eq->item, eq->value);
    }
    case VOCTRL_GET_EQUALIZER: {
        vf_equalizer_t *eq=data;
        return get_equalizer(eq->item, &eq->value);
    }
    case VOCTRL_ONTOP:
        vo_x11_ontop();
        return VO_TRUE;
    case VOCTRL_UPDATE_SCREENINFO:
        update_xinerama_info();
        return VO_TRUE;
    case VOCTRL_DRAW_EOSD:
        if (!data)
            return VO_FALSE;
        generate_eosd(data);
        draw_eosd();
        return VO_TRUE;
    case VOCTRL_GET_EOSD_RES: {
        struct mp_eosd_settings *r = data;
        r->mt = r->mb = r->ml = r->mr = 0;
        r->srcw = vid_width; r->srch = vid_height;
        if (vo_fs) {
            r->w = vo_screenwidth;
            r->h = vo_screenheight;
            r->ml = border_l;
            r->mr = border_r;
            r->mt = border_t;
            r->mb = border_b;
        } else {
            r->w = vo_dwidth;
            r->h = vo_dheight;
        }
        return VO_TRUE;
    }
    }
    return VO_NOTIMPL;
}

/* @} */
