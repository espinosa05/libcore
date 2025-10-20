#ifndef __CORE_OS_LOCK_H__
#define __CORE_OS_LOCK_H__

#include <core/platform.h>
#include <core/cstd.h>

#if defined (PLATFORM_LINUX)
struct os_lock {
    pthread_mutex_t pmutex;
};
#define os_lock_claim(lock_p) pthread_mutex_lock(&lock_p->pmutex)
#define os_lock_release(lock_p) pthread_mutex_unlock(&lock_p->pmutex)
#elif defined (PLATFORM_WINDOWS)
struct os_lock {
    HANDLE wlock;
};
#define os_lock_claim(lock_p)
#define os_lock_release(lock_p)
#else
#   error "platform not supported yet!"
#endif

#endif /* __CORE_OS_LOCK_H__ */
