#ifndef __OS_DYNAMIC_LIBRARY_H__
#define __OS_DYNAMIC_LIBRARY_H__

#include <core/platform.h>

#if defined(CORE_PLATFORM_LINUX)
struct os_library {
    void *handle;
};
#elif defined(CORE_PLATFORM_WINDOWS)
#include <libloaderapi.h>
struct os_library {
    HANDLE handle;
};
#else
#   error "not supported"
#endif /**/

struct os_library_info {
    char *path;
    char *name;
};

void os_open_library(struct os_library *lib, const struct os_library_info info);
void os_load_librarySymbol(struct os_library *lib, void **dst, const char *symbol);
void os_close_library(const struct os_library lib);

#endif /* __OS_DYNAMIC_LIBRARY_H__ */
