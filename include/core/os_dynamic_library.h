#ifndef __OS_DYNAMIC_LIBRARY_H__
#define __OS_DYNAMIC_LIBRARY_H__

#include <core/platform.h>

#if defined(CORE_PLATFORM_LINUX)
struct os_library {
    void *handle;
};

#define OS_LIB_EXT ".so"
#elif defined(CORE_PLATFORM_WINDOWS)
#include <libloaderapi.h>
struct os_library {
    HANDLE handle;
};

#define OS_LIB_EXT ".dll"
#else
#   error "not supported"
#endif /* CORE_PLATFORM_LINUX */

struct os_library_info {
    char *path;
    char *name;
};

void os_library_open(struct os_library *lib, const struct os_library_info info);
void os_library_load_symbol(struct os_library *lib, void **dst, const char *symbol);
void os_library_close(const struct os_library *lib);

#endif /* __OS_DYNAMIC_LIBRARY_H__ */
