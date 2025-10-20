#ifndef __CORE_OS_FILE_H__
#define __CORE_OS_FILE_H__

#include <core/platform.h>
#include <core/cstd.h>
#include <core/types.h>

#if defined(CORE_PLATFORM_LINUX)
struct os_file {
    int handle;
};
enum file_perm {
    FPERM_READ = O_RDONLY,
    FPERM_WRITE = O_WRONLY,
    FPERM_READ_AND_WRITE = O_RDWR,
};

#define OS_FILE_INVALID ((int) -1)

#elif defined(CORE_PLATFORM_WINDOWS)
struct os_file {
    HANDLE handle;
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

extern struct os_file *OS_STDIN;
extern struct os_file *OS_STDOUT;
extern struct os_file *OS_STDERR;

void os_file_open(struct os_file *f, const struct os_file_info info);
void os_file_create(struct os_file *f, const struct os_file_info info);
void os_file_read(const struct os_file *f, void *buffer, const usz buffer_size, const usz bytes);
void os_file_write(const struct os_file *f, const void *buffer, const usz buffer_size, const usz bytes);
void os_file_printf(const struct os_file *f, const char *fmt, ...);
void os_file_close(struct os_file *f);
void os_file_flush(const struct os_file *f);

#endif /* __CORE_OS_FILE_H__ */
