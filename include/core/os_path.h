#ifndef __CORE_OS_PATH_H__
#define __CORE_OS_PATH_H__

#include <core/types.h>
#include <core/memory_macros.h>

struct os_paths {
    MM_ARRAY_MEMBERS(char *);
};

void os_util_strip_file_extension(char *file_name, char **dst);
b32 os_path_exists(char *path);

#endif /* __CORE_OS_UTIL_H__*/
