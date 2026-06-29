#ifndef __CORE_OS_LOCK_H__
#define __CORE_OS_LOCK_H__

#include <core/types.h>
#include <core/platform.h>
#include <core/macros.h>
#include <core/cstd.h>

#if defined (CORE_PLATFORM_LINUX)
#define OS_MUTEX_INIT {PTHREAD_MUTEX_INITIALIZER}
struct os_mutex {
    pthread_mutex_t pmutex;
};

#define os_mutex_init(lock_p) pthread_mutex_init(&(lock_p)->pmutex, NULL)

#define os_mutex_lock(lock_p) pthread_mutex_lock(&(lock_p)->pmutex)

#define os_mutex_unlock(lock_p) pthread_mutex_unlock(&(lock_p)->pmutex)

#define os_mutex_try_lock(lock_p) pthread_mutex_trylock(&(lock_p)->pmutex)

#elif defined (CORE_PLATFORM_WINDOWS)

struct os_mutex {
    HANDLE wmutex;
};

#define os_mutex_lock(lock_p) __EMPTY_MACRO__
#define os_mutex_unlock(lock_p) __EMPTY_MACRO__

#else
#   error "platform not supported yet!"
#endif

#define OS_MUTEX_FMT STR_FMT
#define OS_MUTEX_FMT_ARG(m) os_mutex_try_lock(&m) ? "unlocked" : "locked"

#endif /* __CORE_OS_LOCK_H__ */
