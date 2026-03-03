#ifndef __CORE_OS_UTIL_H__
#define __CORE_OS_UTIL_H__

#include <core/memory_arena.h>

void os_util_strip_file_extension_ar(const char *file_name, char **dst, struct m_arena *arena);

#endif /* __CORE_OS_UTIL_H__ */
