#ifndef __UTILS_H__
#define __UTILS_H__

#include <core/platform.h>
#include <core/log.h>
#include <core/macros.h>
#include <core/os_lock.h>

#ifndef CORE_HEADLESS
#   include <core/native_ui.h>
#else
#   define ASSERT_GUI(...) __EMPTY_MACRO__
#endif /* CORE_HEADLESS */

#if defined(CORE_PLATFORM_LINUX)
#   include "linux_utils.h"
#elif defined(CORE_PLATFORM_WINDOWS)
#   include "windwos_utils.h"
#elif defined(CORE_PLATFORM_MACOS)
#   include "macos_utils.h"
#endif

#define __NULLABLE__

#define TODO(...)       F_LOG_T(OS_STDERR, "TODO", ANSI_COLOR_YELLOW, __VA_ARGS__)
#define IMPL()          TODO("IMPLEMENT FUNCTION")
#define ARRAY_SIZE(a)   (sizeof(a)/sizeof(*a))
#define ABORT()         abort()

#define UNUSED(v)                                               \
    MACRO_START                                                 \
        F_LOG_T(OS_STDERR, "UNUSED", ANSI_COLOR_MAGENTA, #v);   \
        (void)(v);                                              \
    MACRO_END

#define ENUM_STR_ENTRY(e) [e] = #e

#define __TMP_ASSERT(a)                                                         \
    MACRO_START                                                                 \
        if (UNLIKELY(!(a))) {                                                   \
            F_LOG_T(OS_STDERR, "TODO", ANSI_COLOR_RED,                          \
                          "temporary assertion '"#a"' failed!\t");              \
            F_LOG(OS_STDERR, "make sure to implement proper error handling for this one!!"); \
            F_LOG(OS_STDERR, "\n");                                             \
            ABORT();                                                            \
        }                                                                       \
    MACRO_END

#define THROW_EXCEPTION(...)                                            \
    MACRO_START                                                         \
        F_LOG_T(OS_STDERR, "EXCEPTION", ANSI_COLOR_RED, __VA_ARGS__);   \
        F_LOG(OS_STDERR, "\n");                                         \
        ABORT();                                                        \
    MACRO_END

#define UNREACHABLE()                                           \
    MACRO_START                                                 \
        BUILTIN_UNREACHABLE();                                  \
        F_LOG_T(OS_STDERR, "UNREACHABLE", ANSI_COLOR_RED, LOC_FMT, LOC_ARG); \
        ABORT();                                                \
    MACRO_END

#define SWAP(a, b)          \
    MACRO_START             \
        (a) ^= (b);         \
        (b) ^= (a);         \
        (a) ^= (b);         \
    MACRO_END

#define ASSERT_EQ(a, b)                                         \
    MACRO_START                                                 \
        if ((a) != (b)) {                                       \
            F_LOG_T(OS_STDERR, "ASSERT_EQ", ANSI_COLOR_RED,     \
                    "values '"#a"' and '"#b"' don't match!");   \
            ABORT();                                            \
        }                                                       \
    MACRO_END

#define ASSERT_RUN_ONCE()                                                   \
    MACRO_START                                                             \
        static b32 __once_flag = FALSE;                                     \
        static struct os_mutex __once_flag_access_mutex = {0};              \
        os_mutex_lock(&__once_flag_access_mutex);                           \
        ASSERT_RT(FALSE == __once_flag,                                     \
                  "function %s was meant to be called once!", __func__);    \
        os_mutex_unlock(&__once_flag_access_mutex);                         \
    MACRO_END

#define ASSERT_RT(a, ...)                                                   \
    MACRO_START                                                             \
        if (UNLIKELY(!(a))) {                                               \
            LOG_T("ASSERT", ANSI_COLOR_RED, "assertion '"#a"' failed:\t");  \
            LOG(__VA_ARGS__);                                               \
            LOG("\n");                                                      \
            ABORT();                                                        \
        }                                                                   \
    MACRO_END
#define ASSERT(...) ASSERT_RT(__VA_ARGS__)

#define CHECK_NULL(var)                                                     \
    MACRO_START                                                             \
        if (UNLIKELY(!(var))) {                                             \
            LOG_T("CHECK_NULL", ANSI_COLOR_RED, "\""#var"\" is NULL!");     \
            ABORT();                                                        \
        }                                                                   \
    MACRO_END

#ifdef CORE_RELEASE
#   undef ASSERT(...)
#   define ASSERT(a, ...) MACRO_START MACRO_END
#endif /* CORE_RELEASE */


#endif /* __UTILS_H__ */
