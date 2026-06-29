#ifndef __CORE_OS_DIRECTORY_H__
#define __CORE_OS_DIRECTORY_H__

#include <core/types.h>
#include <core/memory.h>
#include <core/memory_arena.h>
#include <core/os_path.h>

struct os_dir_info {
    const char *path;
    usz perm;
};

struct os_dir {
    const char *path;
    int handle;
};

enum os_dir_perm {
    DPERM_READ = O_RDONLY,
};

enum os_path_string_options {
    OS_RELATIVE_PATH = 0,
    OS_ABSOLUTE_PATH,
};

enum os_dir_status_codes {
    OS_DIR_STATUS_SUCCESS = 0,
    OS_DIR_STATUS_FAILURE,
};
typedef usz OS_Dir_Status;

void os_dir_open(struct os_dir *dir, const struct os_dir_info info);
void os_dir_create(struct os_dir *dir, const struct os_dir_info info);
OS_Dir_Status os_dir_get_file_paths(const struct os_dir *dir, struct os_paths *paths);
OS_Dir_Status os_dir_get_file_paths_ar(const struct os_dir *dir, struct os_paths *paths, struct m_arena *arena);

void os_dir_close(const struct os_dir *dir);


#endif /* __CORE_OS_DIRECTORY_H__ */
