/*
 * Network layer for MPlayer
 *
 * Copyright (C) 2001 Bertrand Baudet <bertrand_baudet@yahoo.com>
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

#include "config.h"

#include "libavutil/avstring.h"
#include "mp_msg.h"
#include "help_mp.h"

#if HAVE_WINSOCK2_H
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include "stream.h"
#include "libmpdemux/demuxer.h"
#include "m_config.h"
#include "mpcommon.h"
#include "network.h"
#include "tcp.h"
#include "http.h"
#include "cookies.h"
#include "url.h"

/* Variables for the command line option -user, -passwd, -bandwidth,
   -user-agent and -nocookies */

char *network_username=NULL;
char *network_password=NULL;
int   network_bandwidth=0;
int   network_cookies_enabled = 0;
char *network_useragent=NULL;
char *network_referrer=NULL;
char **network_http_header_fields=NULL;

/* IPv6 options */
int   network_ipv4_only_proxy = 0;


const mime_struct_t mime_type_table[] = {
#ifdef CONFIG_FFMPEG
	// Flash Video
	{ "video/x-flv", DEMUXER_TYPE_LAVF_PREFERRED},
	// do not force any demuxer in this case!
	// we want the lavf demuxer to be tried first (happens automatically anyway),
	// but for mov reference files to work we must also try
	// the native demuxer if lavf fails.
	{ "video/quicktime", 0 },
#endif
	// MP3 streaming, some MP3 streaming server answer with audio/mpeg
	{ "audio/mpeg", DEMUXER_TYPE_AUDIO },
	// MPEG streaming
	{ "video/mpeg", DEMUXER_TYPE_UNKNOWN },
	{ "video/x-mpeg", DEMUXER_TYPE_UNKNOWN },
	{ "video/x-mpeg2", DEMUXER_TYPE_UNKNOWN },
	// AVI ??? => video/x-msvideo
	{ "video/x-msvideo", DEMUXER_TYPE_AVI },
	{ "video/vnd.avi", DEMUXER_TYPE_AVI },
	// MOV => video/quicktime
	{ "video/quicktime", DEMUXER_TYPE_MOV },
	// ASF
        { "audio/x-ms-wax", DEMUXER_TYPE_ASF },
	{ "audio/x-ms-wma", DEMUXER_TYPE_ASF },
	{ "video/x-ms-asf", DEMUXER_TYPE_ASF },
	{ "video/x-ms-afs", DEMUXER_TYPE_ASF },
	{ "video/x-ms-wmv", DEMUXER_TYPE_ASF },
	{ "video/x-ms-wma", DEMUXER_TYPE_ASF },
	{ "application/x-mms-framed", DEMUXER_TYPE_ASF },
	{ "application/vnd.ms.wms-hdr.asfv1", DEMUXER_TYPE_ASF },
	{ "application/octet-stream", DEMUXER_TYPE_UNKNOWN },
	// Playlists
	{ "video/x-ms-wmx", DEMUXER_TYPE_PLAYLIST },
	{ "video/x-ms-wvx", DEMUXER_TYPE_PLAYLIST },
	{ "audio/x-scpls", DEMUXER_TYPE_PLAYLIST },
	{ "audio/x-mpegurl", DEMUXER_TYPE_PLAYLIST },
	{ "audio/x-pls", DEMUXER_TYPE_PLAYLIST },
	// Real Media
//	{ "audio/x-pn-realaudio", DEMUXER_TYPE_REAL },
	// OGG Streaming
	{ "application/ogg", DEMUXER_TYPE_OGG },
	{ "application/x-ogg", DEMUXER_TYPE_OGG },
	{ "audio/ogg", DEMUXER_TYPE_OGG },
	{ "video/ogg", DEMUXER_TYPE_OGG },
	// NullSoft Streaming Video
	{ "video/nsv", DEMUXER_TYPE_NSV},
	{ "misc/ultravox", DEMUXER_TYPE_NSV},
	// HLS Streaming/AppleHttp
	{ "application/vnd.apple.mpegurl", DEMUXER_TYPE_LAVF},
	{ "application/x-mpegURL", DEMUXER_TYPE_LAVF},
	{ NULL, DEMUXER_TYPE_UNKNOWN},
};


streaming_ctrl_t *
streaming_ctrl_new(void) {
	streaming_ctrl_t *streaming_ctrl = calloc(1, sizeof(*streaming_ctrl));
	if( streaming_ctrl==NULL ) {
		mp_msg(MSGT_NETWORK,MSGL_FATAL,MSGTR_MemAllocFailed);
		return NULL;
	}
	return streaming_ctrl;
}

void
streaming_ctrl_free( streaming_ctrl_t *streaming_ctrl ) {
	if( streaming_ctrl==NULL ) return;
	if( streaming_ctrl->url ) url_free( streaming_ctrl->url );
	free(streaming_ctrl->buffer);
	free(streaming_ctrl->data);
	free(streaming_ctrl);
}

URL_t*
check4proxies( const URL_t *url ) {
	URL_t *url_out = NULL;
	if( url==NULL ) return NULL;
	url_out = url_new( url->url );
	if( !av_strcasecmp(url->protocol, "http_proxy") ) {
		mp_msg(MSGT_NETWORK,MSGL_V,"Using HTTP proxy: http://%s:%d\n", url->hostname, url->port );
		return url_out;
	}
	// Check if the http_proxy environment variable is set.
	if( !av_strcasecmp(url->protocol, "http") ) {
		char *proxy;
		proxy = getenv("http_proxy");
		if( proxy!=NULL ) {
			// We got a proxy, build the URL to use it
			char *new_url;
			URL_t *tmp_url;
			URL_t *proxy_url = url_new( proxy );

			if( proxy_url==NULL ) {
				mp_msg(MSGT_NETWORK,MSGL_WARN,
					MSGTR_MPDEMUX_NW_InvalidProxySettingTryingWithout);
				return url_out;
			}

#ifdef HAVE_AF_INET6
			if (network_ipv4_only_proxy && (gethostbyname(url->hostname)==NULL)) {
				mp_msg(MSGT_NETWORK,MSGL_WARN,
					MSGTR_MPDEMUX_NW_CantResolvTryingWithoutProxy);
				url_free(proxy_url);
				return url_out;
			}
#endif

			mp_msg(MSGT_NETWORK,MSGL_V,"Using HTTP proxy: %s\n", proxy_url->url );
			new_url = get_http_proxy_url(proxy_url, url->url);
			if( new_url==NULL ) {
				mp_msg(MSGT_NETWORK,MSGL_FATAL,MSGTR_MemAllocFailed);
				url_free(proxy_url);
				return url_out;
			}
			tmp_url = url_new( new_url );
			if( tmp_url==NULL ) {
				free( new_url );
				url_free( proxy_url );
				return url_out;
			}
			url_free( url_out );
			url_out = tmp_url;
			free( new_url );
			url_free( proxy_url );
		}
	}
	return url_out;
}

URL_t *url_new_with_proxy(const char *urlstr)
{
	URL_t *url = url_new(urlstr);
	URL_t *url_with_proxy = check4proxies(url);
	url_free(url);
	return url_with_proxy;
}

int
http_send_request( URL_t *url, int64_t pos ) {
	HTTP_header_t *http_hdr;
	URL_t *server_url;
	char str[256];
	int fd = -1;
	int ret;
	int proxy = 0;		// Boolean

	http_hdr = http_new_header();

	if( !av_strcasecmp(url->protocol, "http_proxy") ) {
		proxy = 1;
		server_url = url_new( (url->file)+1 );
		if (!server_url) {
			mp_msg(MSGT_NETWORK, MSGL_ERR, "Invalid URL '%s' to proxify\n", url->file+1);
			goto err_out;
		}
		http_set_uri( http_hdr, server_url->noauth_url );
	} else {
		server_url = url;
		http_set_uri( http_hdr, server_url->file );
	}
	if (server_url->port && server_url->port != 80)
	    snprintf(str, sizeof(str), "Host: %s:%d", server_url->hostname, server_url->port );
	else
	    snprintf(str, sizeof(str), "Host: %s", server_url->hostname );
	http_set_field( http_hdr, str);
	if (network_useragent)
	    snprintf(str, sizeof(str), "User-Agent: %s", network_useragent);
	else
	    snprintf(str, sizeof(str), "User-Agent: %s", mplayer_version);
        http_set_field(http_hdr, str);

	if (network_referrer) {
	    char *referrer = NULL;
	    size_t len = strlen(network_referrer) + 10;

	    // Check len to ensure we don't do something really bad in case of an overflow
	    if (len > 10)
		referrer = malloc(len);

	    if (referrer == NULL) {
		mp_msg(MSGT_NETWORK, MSGL_FATAL, MSGTR_MemAllocFailed);
	    } else {
		snprintf(referrer, len, "Referer: %s", network_referrer);
		http_set_field(http_hdr, referrer);
		free(referrer);
	    }
	}

	if( av_strcasecmp(url->protocol, "noicyx") )
	    http_set_field(http_hdr, "Icy-MetaData: 1");

	if(pos>0) {
	// Extend http_send_request with possibility to do partial content retrieval
	    snprintf(str, sizeof(str), "Range: bytes=%"PRId64"-", (int64_t)pos);
	    http_set_field(http_hdr, str);
	}

	if (network_cookies_enabled) cookies_set( http_hdr, server_url->hostname, server_url->url );

	if (network_http_header_fields) {
		int i=0;
		while (network_http_header_fields[i])
			http_set_field(http_hdr, network_http_header_fields[i++]);
	}

	http_set_field( http_hdr, "Connection: close");
	if (proxy)
		http_add_basic_proxy_authentication(http_hdr, url->username, url->password);
	http_add_basic_authentication(http_hdr, server_url->username, server_url->password);
	if( http_build_request( http_hdr )==NULL ) {
		goto err_out;
	}

	if( proxy ) {
		if( url->port==0 ) url->port = 8080;			// Default port for the proxy server
		fd = connect2Server( url->hostname, url->port,1 );
		url_free( server_url );
		server_url = NULL;
	} else {
		if( server_url->port==0 ) server_url->port = 80;	// Default port for the web server
		fd = connect2Server( server_url->hostname, server_url->port,1 );
	}
	if( fd<0 ) {
		goto err_out;
	}
	mp_msg(MSGT_NETWORK,MSGL_DBG2,"Request: [%s]\n", http_hdr->buffer );

	ret = send( fd, http_hdr->buffer, http_hdr->buffer_size, DEFAULT_SEND_FLAGS );
	if( ret!=(int)http_hdr->buffer_size ) {
		mp_msg(MSGT_NETWORK,MSGL_ERR,MSGTR_MPDEMUX_NW_ErrSendingHTTPRequest);
		goto err_out;
	}

	http_free( http_hdr );

	return fd;
err_out:
	if (fd > 0) closesocket(fd);
	http_free(http_hdr);
	if (proxy && server_url)
		url_free(server_url);
	return -1;
}

HTTP_header_t *
http_read_response( int fd ) {
	HTTP_header_t *http_hdr;
	char response[BUFFER_SIZE];
	int i;

	http_hdr = http_new_header();
	if( http_hdr==NULL ) {
		return NULL;
	}

	do {
		i = recv( fd, response, BUFFER_SIZE, 0 );
		if( i<0 ) {
			mp_msg(MSGT_NETWORK,MSGL_ERR,MSGTR_MPDEMUX_NW_ReadFailed);
			http_free( http_hdr );
			return NULL;
		}
		if( i==0 ) {
			mp_msg(MSGT_NETWORK,MSGL_ERR,MSGTR_MPDEMUX_NW_Read0CouldBeEOF);
			http_free( http_hdr );
			return NULL;
		}
		http_response_append( http_hdr, response, i );
	} while( !http_is_header_entire( http_hdr ) );
	if (http_response_parse( http_hdr ) < 0) {
		http_free( http_hdr );
		return NULL;
	}
	return http_hdr;
}

int
http_authenticate(HTTP_header_t *http_hdr, URL_t *url, int *auth_retry) {
	char *aut;

	if( *auth_retry==1 ) {
		mp_msg(MSGT_NETWORK,MSGL_ERR,MSGTR_MPDEMUX_NW_AuthFailed);
		return -1;
	}
	if( *auth_retry>0 ) {
		free(url->username);
		url->username = NULL;
		free(url->password);
		url->password = NULL;
	}

	aut = http_get_field(http_hdr, "WWW-Authenticate");
	if( aut!=NULL ) {
		char *aut_space;
		aut_space = strstr(aut, "realm=");
		if( aut_space!=NULL ) aut_space += 6;
		mp_msg(MSGT_NETWORK,MSGL_INFO,MSGTR_MPDEMUX_NW_AuthRequiredFor, aut_space);
	} else {
		mp_msg(MSGT_NETWORK,MSGL_INFO,MSGTR_MPDEMUX_NW_AuthRequired);
	}
	if( network_username ) {
		url->username = strdup(network_username);
		if( url->username==NULL ) {
			mp_msg(MSGT_NETWORK,MSGL_FATAL,MSGTR_MemAllocFailed);
			return -1;
		}
	} else {
		mp_msg(MSGT_NETWORK,MSGL_ERR,MSGTR_MPDEMUX_NW_AuthFailed);
		return -1;
	}
	if( network_password ) {
		url->password = strdup(network_password);
		if( url->password==NULL ) {
			mp_msg(MSGT_NETWORK,MSGL_FATAL,MSGTR_MemAllocFailed);
			return -1;
		}
	} else {
		mp_msg(MSGT_NETWORK,MSGL_INFO,MSGTR_MPDEMUX_NW_NoPasswdProvidedTryingBlank);
	}
	(*auth_retry)++;
	return 0;
}

int
http_seek( stream_t *stream, int64_t pos ) {
	HTTP_header_t *http_hdr = NULL;
	int fd;
	if( stream==NULL ) return 0;

	if( stream->fd>0 ) closesocket(stream->fd); // need to reconnect to seek in http-stream
	fd = http_send_request( stream->streaming_ctrl->url, pos );
	if( fd<0 ) return 0;

	http_hdr = http_read_response( fd );

	if( http_hdr==NULL ) return 0;

	if( mp_msg_test(MSGT_NETWORK,MSGL_V) )
		http_debug_hdr( http_hdr );

	switch( http_hdr->status_code ) {
		case 200:
		case 206: // OK
			mp_msg(MSGT_NETWORK,MSGL_V,"Content-Type: [%s]\n", http_get_field(http_hdr, "Content-Type") );
			mp_msg(MSGT_NETWORK,MSGL_V,"Content-Length: [%s]\n", http_get_field(http_hdr, "Content-Length") );
			if( http_hdr->body_size>0 ) {
				if( streaming_bufferize( stream->streaming_ctrl, http_hdr->body, http_hdr->body_size )<0 ) {
					http_free( http_hdr );
					return -1;
				}
			}
			break;
		default:
			mp_msg(MSGT_NETWORK,MSGL_ERR,MSGTR_MPDEMUX_NW_ErrServerReturned, http_hdr->status_code, http_hdr->reason_phrase );
			closesocket( fd );
			fd = -1;
	}
	stream->fd = fd;

	if( http_hdr ) {
		http_free( http_hdr );
		stream->streaming_ctrl->data = NULL;
	}

	stream->pos=pos;

	return 1;
}


int
streaming_bufferize( streaming_ctrl_t *streaming_ctrl, char *buffer, int size) {
//printf("streaming_bufferize\n");
	streaming_ctrl->buffer = malloc(size);
	if( streaming_ctrl->buffer==NULL ) {
		mp_msg(MSGT_NETWORK,MSGL_FATAL,MSGTR_MemAllocFailed);
		return -1;
	}
	memcpy( streaming_ctrl->buffer, buffer, size );
	streaming_ctrl->buffer_size = size;
	return size;
}

int
nop_streaming_read( int fd, char *buffer, int size, streaming_ctrl_t *stream_ctrl ) {
	int len=0;
//printf("nop_streaming_read\n");
	if( stream_ctrl->buffer_size!=0 ) {
		int buffer_len = stream_ctrl->buffer_size-stream_ctrl->buffer_pos;
//printf("%d bytes in buffer\n", stream_ctrl->buffer_size);
		len = (size<buffer_len)?size:buffer_len;
		memcpy( buffer, (stream_ctrl->buffer)+(stream_ctrl->buffer_pos), len );
		stream_ctrl->buffer_pos += len;
//printf("buffer_pos = %d\n", stream_ctrl->buffer_pos );
		if( stream_ctrl->buffer_pos>=stream_ctrl->buffer_size ) {
			free( stream_ctrl->buffer );
			stream_ctrl->buffer = NULL;
			stream_ctrl->buffer_size = 0;
			stream_ctrl->buffer_pos = 0;
//printf("buffer cleaned\n");
		}
//printf("read %d bytes from buffer\n", len );
	}

	if( len<size ) {
		int ret;
		ret = recv( fd, buffer+len, size-len, 0 );
		if( ret<0 ) {
			mp_msg(MSGT_NETWORK,MSGL_ERR,"nop_streaming_read error : %s\n",strerror(errno));
			ret = 0;
		} else if (ret == 0)
			stream_ctrl->status = streaming_stopped_e;
		len += ret;
//printf("read %d bytes from network\n", len );
	}

	return len;
}

int
nop_streaming_seek( int fd, int64_t pos, streaming_ctrl_t *stream_ctrl ) {
	return -1;
}


void fixup_network_stream_cache(stream_t *stream) {
  if(stream->streaming_ctrl->buffering) {
    if(stream_cache_size<0) {
      // cache option not set, will use our computed value.
      // buffer in KBytes, *5 because the prefill is 20% of the buffer.
      stream_cache_size = (stream->streaming_ctrl->prebuffer_size/1024)*5;
      if( stream_cache_size<64 ) stream_cache_size = 64;	// 16KBytes min buffer
    }
    mp_msg(MSGT_NETWORK,MSGL_INFO,MSGTR_MPDEMUX_NW_CacheSizeSetTo, stream_cache_size);
  }
}

