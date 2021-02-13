/*
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

#ifndef MPLAYER_MPMEM_H
#define MPLAYER_MPMEM_H

// copy of libavutil/mem_internal.h code for the
// benefit of compiling against shared FFmpeg libs

#include "config.h"

#ifndef DECLARE_ALIGNED
#if defined(__INTEL_COMPILER) && __INTEL_COMPILER < 1110 || defined(__SUNPRO_C)
    #define DECLARE_ALIGNED(n,t,v)      t __attribute__ ((aligned (n))) v
    #define DECLARE_ASM_ALIGNED(n,t,v)  t __attribute__ ((aligned (n))) v
    #define DECLARE_ASM_CONST(n,t,v)    const t __attribute__ ((aligned (n))) v
#elif defined(__DJGPP__)
    #define DECLARE_ALIGNED(n,t,v)      t __attribute__ ((aligned (FFMIN(n, 16)))) v
    #define DECLARE_ASM_ALIGNED(n,t,v)  t av_used __attribute__ ((aligned (FFMIN(n, 16)))) v
    #define DECLARE_ASM_CONST(n,t,v)    static const t av_used __attribute__ ((aligned (FFMIN(n, 16)))) v
#elif defined(__GNUC__) || defined(__clang__)
    #define DECLARE_ALIGNED(n,t,v)      t __attribute__ ((aligned (n))) v
    #define DECLARE_ASM_ALIGNED(n,t,v)  t av_used __attribute__ ((aligned (n))) v
    #define DECLARE_ASM_CONST(n,t,v)    static const t av_used __attribute__ ((aligned (n))) v
#elif defined(_MSC_VER)
    #define DECLARE_ALIGNED(n,t,v)      __declspec(align(n)) t v
    #define DECLARE_ASM_ALIGNED(n,t,v)  __declspec(align(n)) t v
    #define DECLARE_ASM_CONST(n,t,v)    __declspec(align(n)) static const t v
#else
    #define DECLARE_ALIGNED(n,t,v)      t v
    #define DECLARE_ASM_ALIGNED(n,t,v)  t v
    #define DECLARE_ASM_CONST(n,t,v)    static const t v
#endif
#endif

// Some broken preprocessors need a second expansion
// to be forced to tokenize __VA_ARGS__
#define E1(x) x

#define LOCAL_ALIGNED_A(a, t, v, s, o, ...)             \
    uint8_t la_##v[sizeof(t s o) + (a)];                \
    t (*v) o = (void *)FFALIGN((uintptr_t)la_##v, a)

#define LOCAL_ALIGNED_D(a, t, v, s, o, ...)             \
    DECLARE_ALIGNED(a, t, la_##v) s o;                  \
    t (*v) o = la_##v

#define LOCAL_ALIGNED(a, t, v, ...) LOCAL_ALIGNED_##a(t, v, __VA_ARGS__)

#if HAVE_LOCAL_ALIGNED
#   define LOCAL_ALIGNED_4(t, v, ...) E1(LOCAL_ALIGNED_D(4, t, v, __VA_ARGS__,,))
#else
#   define LOCAL_ALIGNED_4(t, v, ...) E1(LOCAL_ALIGNED_A(4, t, v, __VA_ARGS__,,))
#endif

#if HAVE_LOCAL_ALIGNED
#   define LOCAL_ALIGNED_8(t, v, ...) E1(LOCAL_ALIGNED_D(8, t, v, __VA_ARGS__,,))
#else
#   define LOCAL_ALIGNED_8(t, v, ...) E1(LOCAL_ALIGNED_A(8, t, v, __VA_ARGS__,,))
#endif

#if HAVE_LOCAL_ALIGNED
#   define LOCAL_ALIGNED_16(t, v, ...) E1(LOCAL_ALIGNED_D(16, t, v, __VA_ARGS__,,))
#else
#   define LOCAL_ALIGNED_16(t, v, ...) E1(LOCAL_ALIGNED_A(16, t, v, __VA_ARGS__,,))
#endif

#if HAVE_LOCAL_ALIGNED
#   define LOCAL_ALIGNED_32(t, v, ...) E1(LOCAL_ALIGNED_D(32, t, v, __VA_ARGS__,,))
#else
#   define LOCAL_ALIGNED_32(t, v, ...) E1(LOCAL_ALIGNED_A(32, t, v, __VA_ARGS__,,))
#endif

#endif /* MPLAYER_MPMEM_H */
