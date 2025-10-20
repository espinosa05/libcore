#ifndef __CORE_OS_H__
#define __CORE_OS_H__

#include <core/types.h>
#include <core/utils.h>
#include <core/os_streams.h>
typedef usz OS_Thread_Status;
enum os_thread_status_codes {
    OS_THREAD_STATUS_SUCCESS = 0,
    OS_THREAD_STATUS_FAILED_TO_CREATE,

    OS_THREAD_STATUS_FAILURE, /* general failure */
    OS_THREAD_STATUS_UNKNOWN,
    OS_THREAD_STATUS_COUNT,
};

typedef sz os_tid_t;
typedef void (*os_thread_function_t) (void *);

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

OS_Thread_Status os_thread_spawn(struct os_thread *thr, os_thread_function_t func, void *arg);
OS_Thread_Status os_thread_join(struct os_thread *thr, void *ret);
//OS_Thread_Status OS_ThreadGetID(struct os_thread *thr, OS_Tid *tid);
//OS_Thread_Status OS_ThreadDetach(struct os_thread *thr);

usz os_get_max_path_length(const char *path);

/* for accurate Unix-Time value */
void os_time_get_current(struct os_time *time);
/* for precise time measurements */
void os_time_get_monotonic(struct os_time *time);
void os_time_get_diff(struct os_time *result, const struct os_time start, const struct os_time end);

u64 os_time_get_usec(const struct os_time time);
f64 os_time_get_msec(const struct os_time time);
usz os_time_get_sec(const struct os_time time);

void os_sleep_usec(const u32 usec);
void os_sleep_msec(const f32 msec);
void os_sleep_sec(const u32 sec);

#define os_exit(code) exit(code)

#endif /* __V2_CORE_OS_H__ */

