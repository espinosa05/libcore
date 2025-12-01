#ifndef __CORE_OS_THREAD_PLATFORM_H__
#define __CORE_OS_THREAD_PLATFORM_H__

#include <core/platform.h>
#if defined(CORE_PLATFORM_LINUX)
struct os_thread {
    pid_t pid;
};
#else
#   error "platform not supported yet"
#endif /* CORE_PLATFORM_LINUX */

#endif /* __CORE_OS_THREAD_PLATFORM_H__ */
