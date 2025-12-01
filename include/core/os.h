#ifndef __CORE_OS_H__
#define __CORE_OS_H__

#include <core/types.h>
#include <core/utils.h>
#include <core/os_streams.h>
#include <core/os_thread.h>
enum os_exit_codes {
    OS_EXIT_SUCCESS = 0,
    OS_EXIT_FAILURE = 1,
};

#if defined (CORE_PLATFORM_LINUX)
#   include "os_linux_defs.h"
#elif defined (CORE_PLATFORM_WINDOWS)
#   include "os_windows_defs.h"
#elif defined (CORE_PLATFORM_MACOS)
#   include "os_macos_defs.h"
#endif /* CORE_PLATFORM_LINUX */
usz os_get_max_path_length(const char *path);

/* for accurate Unix-Time value */
void os_time_get_current(struct os_time *time);
/* for precise time measurements */
void os_time_get_monotonic(struct os_time *time);
void os_time_get_diff(struct os_time *result, const struct os_time start, const struct os_time end);

u64 os_time_get_usec(const struct os_time time);
f64 os_time_get_msec(const struct os_time time);
usz os_time_get_sec(const struct os_time time);

#define os_exit(code) exit(code)

#endif /* __V2_CORE_OS_H__ */

