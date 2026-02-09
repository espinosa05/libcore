#ifndef __CORE_OS_DIRECTORY_H__
#define __CORE_OS_DIRECTORY_H__

#include <core/memory.h>

struct os_dir_info {
    const char *path;
    usz perm;
};

struct os_dir {
    int handle;
};

enum os_dir_perm {
    DPERM_READ = O_RDONLY,
    DPERM_WRITE = O_WRONLY,
    DPERM_READ_AND_WRITE = O_RDWR,
};

enum os_path_string_options {
    OS_RELATIVE_PATH = 0,
    OS_ABSOLUTE_PATH,
};

#define OS_DIR_INVALID ((int)-1)

void os_dir_open(struct os_dir *dir, const os_dir_info info);
void os_dir_create(struct os_dir *dir, const os_dir_info info);
void os_dir_get_file_paths(const struct os_dir *dir, struct m_array *paths);

void os_dir_close(const struct os_dir *dir);

#endif /* __CORE_OS_DIRECTORY_H__ */
