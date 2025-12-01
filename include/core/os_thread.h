#ifndef __CORE_OS_THREAD_H__
#define __CORE_OS_THREAD_H__

#include <core/os_thread_platform.h>

typedef usz OS_Thread_Status;
enum os_thread_status_codes {
    OS_THREAD_STATUS_SUCCESS = 0,
    OS_THREAD_STATUS_FAILED_TO_CREATE,

    OS_THREAD_STATUS_FAILURE, /* general failure */
    OS_THREAD_STATUS_UNKNOWN,
    OS_THREAD_STATUS_COUNT,
};

OS_Thread_Status os_thread_spawn(struct os_thread *thr, void (*func) (void *), void *arg);
OS_Thread_Status os_thread_join(struct os_thread *thr, void *ret);
OS_Thread_Status os_thread_get_id(struct os_thread *thr, usz *tid);
OS_Thread_Status os_thread_detatch(struct os_thread *thr);

void os_thread_sleep_usec(const u32 usec);
void os_thread_sleep_msec(const u32 msec);
void os_thread_sleep_sec(const u32 sec);

#endif /* __CORE_OS_THREAD_H__ */
