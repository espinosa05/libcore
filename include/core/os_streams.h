#ifndef __CORE_OS_STREAMS_H__
#define __CORE_OS_STREAMS_H__

#include <core/os_file.h>
#include <core/os_socket.h>
#include <core/memory.h>
#include <core/types.h>
#include <core/utils.h>

/* not sure how much of a performance penalty tagged unions have.... but(t) fuck it! */
enum os_stream_types {
    OS_STREAM_TYPE_IPC,
    OS_STREAM_TYPE_NETWORK,
    OS_STREAM_TYPE_FILE,
    OS_STREAM_TYPE_BYTE,
};

struct os_stream_info {
    usz type;
    union {
        struct os_socket_ipc_info   *ipc_info;
        struct os_socket_tcp_info   *tcp_info;
        struct os_file_info         *file_info;
        struct m_buffer_info        *byte_buffer_info;
    } raw;
};

struct os_stream {
    usz type;
    union {
        struct os_file      file;
        struct os_socket    socket;
        struct m_buffer     byte_buffer;
    } raw;
};

enum os_stream_status_codes {
    OS_STREAM_STATUS_SUCCESS = 0,

    OS_STREAM_STATUS_COUNT,
#define OS_STREAM_STATUS_UNKNOWN OS_STREAM_STATUS_COUNT
};
typedef usz OS_Stream_Status;

OS_Stream_Status os_stream_open(struct os_stream *stream, const struct os_stream_info info);
OS_Stream_Status os_stream_close(struct os_stream *stream);
OS_Stream_Status os_stream_read(struct os_stream *stream, void *buffer, const usz buffer_size, const usz size);
OS_Stream_Status os_stream_write(struct os_stream *stream, void *buffer, const usz buffer_size, const usz size);
OS_Stream_Status os_stream_printf(struct os_stream *stream, const char *fmt, ...);

#if 0
OS_StreamStatus OS_StreamOpenAsync(OS_Stream *stream, const OS_StreamCreateInfo *info);
OS_StreamStatus OS_StreamCloseAsync(OS_Stream *stream);
OS_StreamStatus OS_StreamCopyAsync(OS_Stream *dst, OS_Stream *src, usz ammount);
OS_StreamStatus OS_StreamReadAsync(OS_Stream *stream, void *buffer, const usz size);
OS_StreamStatus OS_StreamWriteAsync(OS_Stream *stream, const void *buffer, const usz size);
OS_StreamStatus OS_StreamPrintfAsync(OS_Stream *stream, const char *fmt, ...);
#endif
#endif /* __CORE_OS_STREAMS_H__ */
