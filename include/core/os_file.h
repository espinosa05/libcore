#ifndef __CORE_OS_FILE_H__
#define __CORE_OS_FILE_H__

#include <core/platform.h>
#include <core/types.h>
#include <core/buffer.h>

#if defined(CORE_PLATFORM_LINUX)
#include <core/linux_syscall.h>

#define OS_FILE_SEEK_END    LINUX_SEEK_END
#define OS_FILE_SEEK_SET    LINUX_SEEK_SET
#define OS_FILE_SEEK_CUR    LINUX_SEEK_CUR

struct os_file {
    int handle;
    usz size;
    usz cursor;
};
enum file_perm {
    FPERM_READ = LINUX_O_RDONLY,
    FPERM_WRITE = LINUX_O_WRONLY,
    FPERM_READ_AND_WRITE = LINUX_O_RDWR,
};

#define OS_FILE_INVALID ((int) -1)

#elif defined(CORE_PLATFORM_WINDOWS)
struct os_file {
    HANDLE handle;
    usz size;
    usz cursor;
};
enum file_perm {
    FPERM_READ = GENERIC_READ,
    FPERM_WRITE = GENERIC_WRITE,
    FPERM_READ_AND_WRITE = GENERIC_READ | GENERIC_WRITE,
};

#define OS_FILE_INVALID

#endif

struct os_file_info {
    const char *path;
    usz perm;
};
#define OS_FILE_INFO(...) (struct os_file_info) {__VA_ARGS__}

enum os_file_status_codes {
    OS_FILE_STATUS_SUCCESS = 0,
    OS_FILE_STATUS_PERMISSION_DENIED,
    OS_FILE_STATUS_NO_SUCH_FILE,
    OS_FILE_STATUS_FILE_EXISTS,
    OS_FILE_STATUS_SOURCE_NOT_BIG_ENOUGH,
    OS_FILE_STATUS_DESTINATION_NOT_BIG_ENOUGH,

    OS_FILE_STATUS_COUNT,
#define OS_FILE_STATUS_UNKNOWN OS_FILE_STATUS_COUNT
};

typedef usz OS_File_Status;

extern struct os_file *OS_STDIN;
extern struct os_file *OS_STDOUT;
extern struct os_file *OS_STDERR;

#define OS_FILE_CALL(call)                                                                                              \
    MACRO_START                                                                                                         \
        usz st = (call);                                                                                                \
        if (st != OS_FILE_STATUS_SUCCESS) {                                                                             \
            F_LOG_T(OS_STDERR, "OS_FILE", ANSI_COLOR_RED, "call to '"#call"' failed: "STR_FMT, os_file_status_str(st)); \
            ABORT();                                                                                                    \
        }                                                                                                               \
    MACRO_END

OS_File_Status os_file_open(struct os_file *f, const struct os_file_info info);
OS_File_Status os_file_create(struct os_file *f, const struct os_file_info info);
OS_File_Status os_file_read(const struct os_file *f, void *buffer, const usz bytes);
OS_File_Status os_file_write(const struct os_file *f, const void *buffer, const usz bytes);

OS_File_Status os_file_read_be8(struct os_file *f, u8 *buffer);
OS_File_Status os_file_read_be16(struct os_file *f, u16 *buffer);
OS_File_Status os_file_read_be32(struct os_file *f, u32 *buffer);
OS_File_Status os_file_read_be64(struct os_file *f, u64 *buffer);

OS_File_Status os_file_write_be8(struct os_file *f, u8 val);
OS_File_Status os_file_write_be16(struct os_file *f, u16 val);
OS_File_Status os_file_write_be32(struct os_file *f, u32 val);
OS_File_Status os_file_write_be64(struct os_file *f, u64 val);

OS_File_Status os_file_read_le8(struct os_file *f, u8 *buffer);
OS_File_Status os_file_read_le16(struct os_file *f, u16 *buffer);
OS_File_Status os_file_read_le32(struct os_file *f, u32 *buffer);
OS_File_Status os_file_read_le64(struct os_file *f, u64 *buffer);

OS_File_Status os_file_write_le8(struct os_file *f, u8 val);
OS_File_Status os_file_write_le16(struct os_file *f, u16 val);
OS_File_Status os_file_write_le32(struct os_file *f, u32 val);
OS_File_Status os_file_write_le64(struct os_file *f, u64 val);

OS_File_Status os_file_read_buff(struct os_file *f, usz bytes, struct m_buffer *buff);
OS_File_Status os_file_write_buff(struct os_file *f, usz bytes, struct m_buffer *buff);

usz os_file_seek(struct os_file *f, usz n, usz whence, usz *dst);
usz os_file_get_size(struct os_file *f);

OS_File_Status os_file_printf(const struct os_file *f, const char *fmt, ...);
OS_File_Status os_file_close(struct os_file *f);
void os_file_flush(const struct os_file *f);
char *os_file_status_str(usz st);

#endif /* __CORE_OS_FILE_H__ */
