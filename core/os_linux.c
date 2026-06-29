#include <core/os.h>
#include <core/os_dynamic_library.h>
#include <core/os_streams.h>
#include <core/os_file.h>
#include <core/os_path.h>
#include <core/os_directory.h>
#include <core/os_socket.h>
#include <core/os_thread.h>
#include <core/utils.h>
#include <core/strings.h>
#include <core/types.h>
#include <core/memory.h>
#include <core/cstd.h>
#include <core/linux.h>
#include <core/linux_syscall.h>
#include <core/vulkan.h>

/* static function declaration start */
static OS_Stream_Status open_file_stream(struct os_stream *stream, const struct os_file_info info);
static OS_Stream_Status open_tcp_socket_stream(struct os_stream *stream, const struct os_socket_tcp_listener_info info);
static OS_Stream_Status open_ipc_socket_stream(struct os_stream *stream, const struct os_socket_ipc_info info);

static void os_file_init_proc_io_handles(void);
static void set_coredumps(void);
static void reset_coredumps(void);

static void platform_shutdown(void) __FUNC_ATTR_DESTRUCTOR__;
static void platform_init(void) __FUNC_ATTR_CONSTRUCTOR__;
static void register_core_signal_handlers(void) __FUNC_ATTR_CONSTRUCTOR__;
static void core_empty_handler(int sig_num);
static void set_linux_signal_handler(int sig_num, void (*handler) (int));

static OS_File_Status os_file_errno_to_status(usz function, usz errno_val);
/* static function declaration end */

/* global data start */
struct os_file *OS_STDIN;
struct os_file *OS_STDOUT;
struct os_file *OS_STDERR;

static struct os_file g_os_stdin;
static struct os_file g_os_stdout;
static struct os_file g_os_stderr;

#if COMMENT
static struct rlimit initial_coredump_count = {0};
#endif
/* global data end */

void os_library_open(struct os_library *lib, const struct os_library_info info)
{
    ASSERT(info.name, "No library name passed!");

    char *path = info.path;
    if (!path)
        path = "./";

    char *actual_path_str = NULL;
    {
        struct str_builder whole_path = {0};
        str_builder_init(&whole_path, os_get_sane_path_length(path));
        str_builder_append(&whole_path, path);

        if ('/' == cstr_char_at_backwards(path, 0))
            str_builder_append(&whole_path, "/");

        str_builder_append(&whole_path, info.name);
        str_builder_append(&whole_path, ".so");
        str_builder_to_cstr_alloc(&whole_path, &actual_path_str);
        str_builder_delete(whole_path);
    }

    lib->handle = dlopen(actual_path_str, RTLD_LAZY);
    ASSERT_RT(lib->handle, "failed to open dynamic library "STR_QUOT_LIT(STR_FMT)" "STR_FMT, actual_path_str, dlerror());

    m_free(actual_path_str);
}

void os_library_load_symbol(struct os_library *lib, void **dst, const char *symbol)
{
   *dst = dlsym(lib->handle, symbol);
   ASSERT_RT(*dst, "failed to load symbol"STR_QUOT_LIT(STR_FMT)" "STR_FMT, symbol, dlerror());
}

void os_library_close(const struct os_library *lib)
{
    int st = dlclose(lib->handle);
    ASSERT_RT(st == 0, "failed to close dynamic library: "STR_FMT, dlerror());
}

OS_Thread_Status os_thread_spawn(struct os_thread *thr, void (*func) (void *), void *arg)
{
    struct linux_clone_args clone_args = {0};
    struct linux_rlimit stack_size = {0};
    void *stack_buffer = NULL;

    /* getrlimit does not return useful errors */
    linux_getrlimit(RLIMIT_STACK, &stack_size);

    stack_buffer = linux_mmap(NULL, stack_size.rlim_curr, PROT_READ | PROT_WRITE,
                              MAP_GROWSDOWN | MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);

    if (LINUX_MMAP_FAILURE(stack_buffer))
        return OS_THREAD_STATUS_FAILURE;

    clone_args.flags         = LINUX_CLONE_VM | LINUX_CLONE_FS | LINUX_CLONE_FILES | LINUX_CLONE_SIGHAND;
    clone_args.exit_signal   = LINUX_SIGCHLD;
    clone_args.stack         = U64(stack_buffer);
    clone_args.stack_size    = stack_size.rlim_curr;

    thr->pid = syscall(SYS_clone3, &clone_args, sizeof(clone_args));
    if (LINUX_SYSCALL_FAILURE(thr->pid))
        return OS_THREAD_STATUS_FAILURE;

    if (thr->pid != 0) {
        func(arg);
    }

    return OS_THREAD_STATUS_SUCCESS;
}

OS_Thread_Status os_thread_join(struct os_thread *thr)
{
    int status = 0;
    linux_waitpid(thr->pid, &status, 0);

    return OS_THREAD_STATUS_SUCCESS;
}

void os_time_get_current(struct os_time *time)
{
    ssz st = linux_clock_gettime(CLOCK_REALTIME, &time->time_val);

    if (LINUX_SYSCALL_FAILURE(st)) {
        THROW_EXCEPTION("couldn't get system time: "STR_FMT, linux_errno_cstr(-st));
    }
}

void os_time_get_monotonic(struct os_time *time)
{
    ssz st = linux_clock_gettime(CLOCK_MONOTONIC, &time->time_val);
    if (LINUX_SYSCALL_FAILURE(st)) {
        THROW_EXCEPTION("couldn't get system time: "STR_FMT, linux_errno_cstr(-st));
    }
}

void os_time_get_diff(struct os_time *result, const struct os_time start, const struct os_time end)
{
    result->time_val.tv_sec = end.time_val.tv_sec - start.time_val.tv_sec;
    result->time_val.tv_nsec = end.time_val.tv_nsec - start.time_val.tv_nsec;
}

usz os_time_get_nsec(const struct os_time time)
{
    return (time.time_val.tv_sec) * 1000000000 + (time.time_val.tv_nsec);
}

u64 os_time_get_usec(const struct os_time time)
{
    return os_time_get_nsec(time)/1000;
}

f64 os_time_get_msec(const struct os_time time)
{
    return F64(os_time_get_usec(time))/1000;
}

usz os_time_get_sec(const struct os_time time)
{
    return USZ(os_time_get_msec(time))/1000;
}

void os_thread_sleep_usec(u32 usec)
{
    usleep(usec);
}

void os_thread_sleep_msec(u32 msec)
{
    os_thread_sleep_usec((msec * 1000));
}

void os_thread_sleep_sec(u32 sec)
{
    /* we don't call 'os_thread_sleep_msec',
     * as 1000000 usec appears to trigger EINVAL on some systems
     * */
    sleep((unsigned int)sec);
}

usz os_get_max_path_length(const char *path)
{
    return pathconf(path, _PC_PATH_MAX);
}

/* os_get_max_path_length may return absurdly large values so this is preferred instead */
usz os_get_sane_path_length(const char *path)
{
    usz length = os_get_max_path_length(path);

    if (length > KB_SIZE)
        length = KB_SIZE;

    return length;
}

OS_Stream_Status os_stream_open(struct os_stream *stream, const struct os_stream_info info)
{
    OS_Stream_Status st = OS_STREAM_STATUS_UNKNOWN;
    switch (info.type) {
    case OS_STREAM_TYPE_IPC:
        st = open_ipc_socket_stream(stream, *info.raw.ipc_info);
        break;
    case OS_STREAM_TYPE_NETWORK:
        st = open_tcp_socket_stream(stream, *info.raw.tcp_info);
        break;
    case OS_STREAM_TYPE_FILE:
        st = open_file_stream(stream, *info.raw.file_info);
        break;
    }

    return st;
}

OS_Stream_Status os_stream_close(struct os_stream *stream)
{
    switch (stream->type) {
    case OS_STREAM_TYPE_IPC:
    case OS_STREAM_TYPE_NETWORK:
        os_socket_close(&stream->raw.socket);
        break;
    case OS_STREAM_TYPE_FILE:
        os_file_close(&stream->raw.file);
        break;
    }

    return OS_STREAM_STATUS_SUCCESS;
}

OS_Stream_Status os_stream_read(struct os_stream *stream, void *buffer, const usz bytes)
{
    switch (stream->type) {
    case OS_STREAM_TYPE_IPC:
    case OS_STREAM_TYPE_NETWORK:
        os_socket_receive_data(&stream->raw.socket, buffer, bytes, 0);
        break;
    case OS_STREAM_TYPE_FILE:
        os_file_read(&stream->raw.file, buffer, bytes);
        break;
    case OS_STREAM_TYPE_BYTE:
        m_buffer_read(&stream->raw.byte_buffer, buffer, bytes);
        break;
    }

    return OS_STREAM_STATUS_SUCCESS;
}

enum os_file_functions {
    OS_FILE_FUNC_OPEN = 0,
    OS_FILE_FUNC_CREATE,
    OS_FILE_FUNC_READ,
    OS_FILE_FUNC_WRITE,
    OS_FILE_FUNC_CLOSE,
    OS_FILE_FUNC_LSEEK,
};

static OS_File_Status os_file_errno_to_status(usz function, usz errno_val)
{
    if (errno_val == 0)
        return OS_FILE_STATUS_SUCCESS;

    switch (function) {
    case OS_FILE_FUNC_OPEN:
        switch (errno_val) {
        case EACCES: return OS_FILE_STATUS_PERMISSION_DENIED;
        case ENOENT: return OS_FILE_STATUS_NO_SUCH_FILE;
        default: UNREACHABLE();
        }
    case OS_FILE_FUNC_CREATE:
        switch (errno_val) {
        case EACCES: return OS_FILE_STATUS_PERMISSION_DENIED;
        case EEXIST: return OS_FILE_STATUS_FILE_EXISTS;
        default: UNREACHABLE();
        }
    case OS_FILE_FUNC_READ:
    case OS_FILE_FUNC_WRITE:
    case OS_FILE_FUNC_CLOSE:
    case OS_FILE_FUNC_LSEEK:
        switch (errno_val) {
        default: UNREACHABLE();
        }
    default: UNREACHABLE();
    }
}

OS_Stream_Status os_stream_write(struct os_stream *stream, void *buffer, const usz bytes)
{
    switch (stream->type) {
    case OS_STREAM_TYPE_IPC:
    case OS_STREAM_TYPE_NETWORK:
        os_socket_send_data(&stream->raw.socket, buffer, bytes, 0);
        break;
    case OS_STREAM_TYPE_FILE:
        os_file_write(&stream->raw.file, buffer, bytes);
        break;
    case OS_STREAM_TYPE_BYTE:
        m_buffer_write(&stream->raw.byte_buffer, buffer, bytes);
        break;
    }

    return OS_STREAM_STATUS_SUCCESS;
}


OS_Stream_Status os_stream_read_buff(struct os_stream *stream, struct m_buffer *buff, const usz bytes)
{
    UNUSED(stream);
    UNUSED(buff);
    UNUSED(bytes);
    IMPL();

    return OS_STREAM_STATUS_SUCCESS;
}

OS_Stream_Status os_stream_write_buff(struct os_stream *stream, struct m_buffer *buff, const usz bytes)
{
    UNUSED(stream);
    UNUSED(buff);
    UNUSED(bytes);
    IMPL();

    return OS_STREAM_STATUS_SUCCESS;
}

OS_Stream_Status os_stream_printf(struct os_stream *stream, const char *fmt, ...)
{
    char *buffer = NULL;
    va_list args;
    usz mesage_length;

    va_start(args, fmt);
    cstr_format_alloc_variadic(&buffer, fmt, args, &mesage_length);
    os_stream_write(stream, buffer, mesage_length);

    m_free(buffer);

    return OS_STREAM_STATUS_SUCCESS;
}

OS_File_Status os_file_open(struct os_file *f, const struct os_file_info info)
{
    ssz st = linux_open(info.path, info.perm);
    if (LINUX_SYSCALL_FAILURE(st)) {
        f->handle = OS_FILE_INVALID;
        return os_file_errno_to_status(OS_FILE_FUNC_OPEN, USZ(-st));
    }

    f->handle = st;

    return OS_FILE_STATUS_SUCCESS;
}

OS_File_Status os_file_create(struct os_file *f, const struct os_file_info info)
{
    u32 fd_perm = info.perm | LINUX_O_CREAT; /* the user does not need to know this happens */
    ssz st = linux_open_mode(info.path, fd_perm, LINUX_S_IWUSR);
    if (LINUX_SYSCALL_FAILURE(st)) {
        f->handle = OS_FILE_INVALID;
        return os_file_errno_to_status(OS_FILE_FUNC_CREATE, USZ(-st));
    }

    f->handle = st;

    return OS_FILE_STATUS_SUCCESS;
}

OS_File_Status os_file_read(const struct os_file *f, void *buffer, const usz bytes)
{
    ssz st = linux_read(f->handle, buffer, bytes);
    if (LINUX_SYSCALL_FAILURE(st)) {
        return os_file_errno_to_status(OS_FILE_FUNC_READ, USZ(-st));
    }

    return OS_FILE_STATUS_SUCCESS;
}

OS_File_Status os_file_write(const struct os_file *f, const void *buffer, const usz bytes)
{
    ssz st = linux_write(f->handle, buffer, bytes);
    if (LINUX_SYSCALL_FAILURE(st)) {
        return os_file_errno_to_status(OS_FILE_FUNC_WRITE, USZ(-st));
    }

    return OS_FILE_STATUS_SUCCESS;
}

OS_File_Status os_file_read_buff(struct os_file *f, usz bytes, struct m_buffer *buff)
{
    if (bytes > (buff->size - buff->cursor)) {
        return OS_FILE_STATUS_DESTINATION_NOT_BIG_ENOUGH;
    }

    ssz st = linux_read(f->handle, buff->base, bytes);
    if (LINUX_SYSCALL_FAILURE(st)) {
        return os_file_errno_to_status(OS_FILE_FUNC_WRITE, USZ(-st));
    }

    return OS_FILE_STATUS_SUCCESS;
}

OS_File_Status os_file_write_buff(struct os_file *f, usz bytes, struct m_buffer *buff)
{
    if (bytes > (buff->size - buff->cursor)) {
        return OS_FILE_STATUS_SOURCE_NOT_BIG_ENOUGH;
    }

    ssz st = linux_write(f->handle, buff->base, bytes);
    if (LINUX_SYSCALL_FAILURE(st)) {
        return os_file_errno_to_status(OS_FILE_FUNC_READ, USZ(-st));
    }

    return OS_FILE_STATUS_SUCCESS;
}

OS_File_Status os_file_read_le8(struct os_file *f, u8 *buffer)
{
    return os_file_read(f, buffer, sizeof(*buffer));
}

OS_File_Status os_file_read_le16(struct os_file *f, u16 *buffer)
{
    usz st = os_file_read(f, buffer, sizeof(*buffer));
    u8 *bytes = U8_PTR(buffer);
    SWAP(bytes[0], bytes[1]);
    return st;
}

OS_File_Status os_file_read_le32(struct os_file *f, u32 *buffer)
{
    usz st = os_file_read(f, buffer, sizeof(*buffer));
    u8 *bytes = U8_PTR(buffer);
    SWAP(bytes[0], bytes[3]);
    SWAP(bytes[1], bytes[2]);
    return st;
}

OS_File_Status os_file_read_le64(struct os_file *f, u64 *buffer)
{
    usz st = os_file_read(f, buffer, sizeof(*buffer));
    u8 *bytes = U8_PTR(buffer);
    SWAP(bytes[0], bytes[7]);
    SWAP(bytes[1], bytes[6]);
    SWAP(bytes[2], bytes[5]);
    SWAP(bytes[3], bytes[4]);
    return st;
}

OS_File_Status os_file_write_be8(struct os_file *f, u8 val)
{
    return os_file_write(f, &val, sizeof(val));
}

OS_File_Status os_file_write_be16(struct os_file *f, u16 val)
{
    u8 *bytes = U8_PTR(&val);
    SWAP(bytes[0], bytes[1]);
    return os_file_write(f, &val, sizeof(val));
}

OS_File_Status os_file_write_be32(struct os_file *f, u32 val)
{
    u8 *bytes = U8_PTR(&val);
    SWAP(bytes[0], bytes[3]);
    SWAP(bytes[1], bytes[2]);
    return os_file_write(f, &val, sizeof(val));
}

OS_File_Status os_file_write_be64(struct os_file *f, u64 val)
{
    u8 *bytes = U8_PTR(&val);
    SWAP(bytes[0], bytes[7]);
    SWAP(bytes[1], bytes[6]);
    SWAP(bytes[2], bytes[5]);
    SWAP(bytes[3], bytes[4]);
    return os_file_write(f, &val, sizeof(val));
}

OS_File_Status os_file_read_be8(struct os_file *f, u8 *buffer)
{
    return os_file_read(f, buffer, sizeof(*buffer));
}

OS_File_Status os_file_read_be16(struct os_file *f, u16 *buffer)
{
    return os_file_read(f, buffer, sizeof(*buffer));
}

OS_File_Status os_file_read_be32(struct os_file *f, u32 *buffer)
{
    return os_file_read(f, buffer, sizeof(*buffer));
}

OS_File_Status os_file_read_be64(struct os_file *f, u64 *buffer)
{
    return os_file_read(f, buffer, sizeof(*buffer));
}

OS_File_Status os_file_write_le8(struct os_file *f, u8 val)
{
    return os_file_write(f, &val, sizeof(val));
}

OS_File_Status os_file_write_le16(struct os_file *f, u16 val)
{
    return os_file_write(f, &val, sizeof(val));
}

OS_File_Status os_file_write_le32(struct os_file *f, u32 val)
{
    return os_file_write(f, &val, sizeof(val));
}

OS_File_Status os_file_write_le64(struct os_file *f, u64 val)
{
    return os_file_write(f, &val, sizeof(val));
}

usz os_file_seek(struct os_file *f, usz n, usz whence, usz *dst)
{
    ssz st = linux_lseek(f->handle, n, whence);
    if (LINUX_SYSCALL_FAILURE(st)) {
        return os_file_errno_to_status(OS_FILE_FUNC_LSEEK, USZ(-st));
    }

    if (dst) {
        *dst = st;
    }

    return OS_FILE_STATUS_SUCCESS;
}

usz os_file_get_size(struct os_file *f)
{
    usz size = 0;
    os_file_seek(f, 0, OS_FILE_SEEK_END, &size);
    os_file_seek(f, 0, OS_FILE_SEEK_SET, NULL);

    return size;
}

OS_File_Status os_file_printf(const struct os_file *f, const char *fmt, ...)
{
    char *msg;
    va_list args;
    usz msg_length = 0;

    va_start(args, fmt);
    cstr_format_alloc_variadic(&msg, fmt, args, &msg_length);
    usz st = os_file_write(f, msg, msg_length);
    m_free(msg);

    return st;
}

OS_File_Status os_file_close(struct os_file *f)
{
    ssz st = linux_close(f->handle);

    if (!LINUX_SYSCALL_SUCCESS(st))
        return os_file_errno_to_status(OS_FILE_FUNC_CLOSE, USZ(-st));

    f->handle = OS_FILE_INVALID;

    return OS_FILE_STATUS_SUCCESS;
}

void os_file_flush(const struct os_file *f)
{
    IMPL();
    UNUSED(f);
}

static char *g_os_file_status_strs[] = {
    ENUM_STR_ENTRY(OS_FILE_STATUS_SUCCESS),
    ENUM_STR_ENTRY(OS_FILE_STATUS_PERMISSION_DENIED),
    ENUM_STR_ENTRY(OS_FILE_STATUS_NO_SUCH_FILE),
    ENUM_STR_ENTRY(OS_FILE_STATUS_FILE_EXISTS),
    ENUM_STR_ENTRY(OS_FILE_STATUS_SOURCE_NOT_BIG_ENOUGH),
    ENUM_STR_ENTRY(OS_FILE_STATUS_DESTINATION_NOT_BIG_ENOUGH),
    ENUM_STR_ENTRY(OS_FILE_STATUS_UNKNOWN),
};

char *os_file_status_str(usz st)
{
    if (st > OS_FILE_STATUS_COUNT)
        st = OS_FILE_STATUS_UNKNOWN;

    return g_os_file_status_strs[st];
}

b32 os_path_exists(char *path)
{
    struct linux_stat path_stat = {0};
    ssz st = linux_stat(path, &path_stat);
    return USZ(-st) != ENOENT;
}

void os_dir_open(struct os_dir *dir, const struct os_dir_info info)
{
    u32 perm = info.perm | O_DIRECTORY;
    ssz st = linux_open(info.path, perm);

    ASSERT_RT(LINUX_SYSCALL_SUCCESS(st), "failed to open directory! "STR_FMT, linux_errno_cstr(USZ(-st)));

    dir->path = info.path;
    dir->handle = st;
}

void os_dir_create(struct os_dir *dir, const struct os_dir_info info)
{
    u64 mode = S_IRUSR;
    ssz st = linux_mkdir(info.path, mode);
    ASSERT_RT(LINUX_SYSCALL_SUCCESS(st), "failed to create directory!");

    os_dir_open(dir, info);
}

#define INIT_PATH_ARRAY_COUNT 16
#define DONE_READING_DIR_BUFF 0
OS_Dir_Status os_dir_get_file_paths(const struct os_dir *dir, struct os_paths *paths)
{
    usz path_cap = os_get_sane_path_length(dir->path);
    OS_Dir_Status ret = OS_DIR_STATUS_SUCCESS;

    usz buff_size = sizeof(struct linux_dirent64) + path_cap + NULL_TERM_SIZE;
    struct m_buffer buffer = {0};
    struct m_buffer_info buffer_info = {0};
    buffer_info.size = buff_size;
    m_buffer_init(&buffer, buffer_info);

    struct linux_dirent64 *dir_entry = NULL;

    /* get directory records from Kernel */
    while (TRUE) {
        usz bytes_read = 0;
        ssz st = linux_getdents64(dir->handle, buffer.base, buffer.size);
        if (LINUX_SYSCALL_FAILURE(st)) {
            ERROR_LOG("failed to get directory entry : "STR_FMT, linux_errno_cstr(USZ(-st)));
            ret = OS_DIR_STATUS_FAILURE;
            break;
        }

        bytes_read = USZ(st);
        buffer.cursor += bytes_read;
        if (st == DONE_READING_DIR_BUFF) {
            break;
        }

        m_buffer_resize(&buffer, buffer.size + KB_SIZE/2);
    }

    mm_array_init(paths, INIT_PATH_ARRAY_COUNT);

    /* retreive entries from buffer */
    for (usz offset = 0; offset < buffer.cursor; offset += dir_entry->d_reclen) {
        dir_entry = LINUX_DIRENT64_REF(U8_PTR(buffer.base) + offset);
        if (dir_entry->d_type == DT_REG) {
            mm_array_append(paths, cstr_duplicate(dir_entry->d_name));
        }
    }

    m_buffer_delete(&buffer);

    return ret;
}

OS_Dir_Status os_dir_get_file_paths_ar(const struct os_dir *dir, struct os_paths *paths, struct m_arena *arena)
{
    usz path_cap = os_get_sane_path_length(dir->path);
    OS_Dir_Status ret = OS_DIR_STATUS_SUCCESS;

    usz buff_size = sizeof(struct linux_dirent64) + path_cap + NULL_TERM_SIZE;
    struct m_buffer buffer = {0};
    struct m_buffer_info buffer_info = {0};
    buffer_info.size = buff_size;
    m_buffer_init(&buffer, buffer_info);

    struct linux_dirent64 *dir_entry = NULL;

    /* get directory records from Kernel */
    while (TRUE) {
        usz bytes_read = 0;
        ssz st = linux_getdents64(dir->handle, buffer.base, buffer.size);
        if (LINUX_SYSCALL_FAILURE(st)) {
            ERROR_LOG("failed to get directory entry : "STR_FMT, linux_errno_cstr(USZ(-st)));
            ret = OS_DIR_STATUS_FAILURE;
            break;
        }

        bytes_read = USZ(st);
        buffer.cursor += bytes_read;
        if (st == DONE_READING_DIR_BUFF) {
            break;
        }

        m_buffer_resize(&buffer, buffer.size + KB_SIZE/2);
    }

    /* count entries */
    usz entry_count = 0;
    for (usz offset = 0; offset < buffer.cursor; offset += dir_entry->d_reclen) {
        dir_entry = LINUX_DIRENT64_REF(U8_PTR(buffer.base) + offset);
        ++entry_count;
    }
    mm_array_init_ar(paths, entry_count, arena);

    /* retreive entries from buffer */
    for (usz offset = 0; offset < buffer.cursor; offset += dir_entry->d_reclen) {
        dir_entry = LINUX_DIRENT64_REF(U8_PTR(buffer.base) + offset);
        if (dir_entry->d_type == DT_REG) {
            mm_array_append(paths, cstr_duplicate(dir_entry->d_name));
        }
    }

    return ret;
}

void os_dir_cleanup_paths(const struct os_paths paths)
{
    /* free allocated path strings */
    for (usz i = 0; i < paths.count; ++i) {
        m_free(paths.data[i]);
    }
    m_free(paths.data);
}

void os_dir_close(const struct os_dir *dir)
{
    linux_close(dir->handle);
}


#define OS_SOCKET_INVALID FD_INVALID

enum socket_functions {
    SOCK_FN_SOCKET,
    SOCK_FN_BIND,
    SOCK_FN_LISTEN,
    SOCK_FN_ACCEPT,
    SOCK_FN_CONNECT,
    SOCK_FN_SEND,
    SOCK_FN_RECV,
    SOCK_FN_SENDTO,
    SOCK_FN_RECVFROM,
    SOCK_FN_SENDMSG,
    SOCK_FN_RECVMSG,
    SOCK_FN_SHUTDOWN,
    SOCK_FN_CLOSE,
    SOCK_FN_GETSOCKOPT,
    SOCK_FN_SETSOCKOPT,
    SOCK_FN_GETSOCKNAME,
    SOCK_FN_GETPEERNAME,
    SOCK_FN_SELECT,
    SOCK_FN_POLL,
    SOCK_FN_EPOLL_CREATE,
    SOCK_FN_EPOLL_CTL,
    SOCK_FN_EPOLL_WAIT,
    SOCK_FN_ACCEPT4,
};

#define SOCKOPT_ENABLE &(int) {1}
#define SOCKOPT_DISABLE &(int) {0}
OS_Socket_Status os_socket_enable_sockopt(const struct os_socket *sock, sz sock_opt)
{
    ssz st = linux_setsockopt(sock->handle, SOL_SOCKET, sock_opt, SOCKOPT_ENABLE, sizeof(int));
    return os_socket_errno_code_to_status(SOCK_FN_SETSOCKOPT, USZ(-st));
}

OS_Socket_Status os_socket_disable_sock_opt(const struct os_socket *sock, sz sock_opt)
{
    const usz socklen = sizeof(int);
    ssz st = linux_setsockopt(sock->handle, SOL_SOCKET, sock_opt, SOCKOPT_DISABLE, socklen);
    return os_socket_errno_code_to_status(SOCK_FN_SETSOCKOPT, USZ(-st));
}

OS_Socket_Status os_socket_get_sockopt(const struct os_socket *sock, sz sock_opt, sz *value)
{
    usz socklen = sizeof(int);
    ssz st = linux_getsockopt(sock->handle, SOL_SOCKET, sock_opt, value, &socklen);
    return os_socket_errno_code_to_status(SOCK_FN_GETSOCKOPT, USZ(-st));
}

OS_Socket_Status os_socket_set_sockobj(const struct os_socket *sock, sz sock_opt, const void *sock_obj, usz size)
{
    ssz st = linux_setsockopt(sock->handle, SOL_SOCKET, sock_opt, sock_obj, size);
    return os_socket_errno_code_to_status(SOCK_FN_SETSOCKOPT, USZ(-st));
}

OS_Socket_Status os_socket_get_sockobj(const struct os_socket *sock, sz sock_opt, void *sock_obj, usz size)
{
    ssz st = getsockopt(sock->handle, SOL_SOCKET, sock_opt, sock_obj, &(socklen_t) {size});
    return os_socket_errno_code_to_status(SOCK_FN_GETSOCKOPT, USZ(-st));
}

OS_Socket_Status os_socket_create_tcp_listener(struct os_socket *sock, const struct os_socket_tcp_listener_info info)
{
    CHECK_NULL(info.net_addr);

    struct linux_sockaddr_in socket_address = {0};
    ssz st = 0;

    st = linux_socket(LINUX_SOCKET_AF_INET, LINUX_SOCKET_TYPE_STREAM, LINUX_SOCKET_PROTOCOL_TYPE_TCP);
    if (!LINUX_SYSCALL_SUCCESS(st))
        return os_socket_errno_code_to_status(SOCK_FN_SOCKET, USZ(-st));

    sock->handle = st;
    socket_address.family   = LINUX_SOCKET_AF_INET;
    socket_address.addr     = net_u32(info.net_addr->ipv4);
    socket_address.port     = net_u16(info.net_addr->port);

    st = linux_bind(sock->handle, &socket_address, sizeof(socket_address));
    if (!LINUX_SYSCALL_SUCCESS(st))
        return os_socket_errno_code_to_status(SOCK_FN_BIND, USZ(-st));

    st = linux_listen(sock->handle, info.queue_length);
    if (!LINUX_SYSCALL_SUCCESS(st))
        return os_socket_errno_code_to_status(SOCK_FN_LISTEN, USZ(-st));

    return OS_SOCKET_STATUS_SUCCESS;
}

OS_Socket_Status os_socket_create_ipc_socket(struct os_socket *sock, const struct os_socket_ipc_info info)
{
    UNUSED(sock);
    UNUSED(info);
    IMPL();
    return OS_SOCKET_STATUS_SUCCESS;
}

OS_Socket_Status os_socket_bind_path(const struct os_socket *sock)
{
    UNUSED(sock);
    return OS_SOCKET_STATUS_SUCCESS;
}

OS_Socket_Status os_socket_accept(const struct os_socket *server, struct os_socket *client, struct net_address *address)
{
    ssz st = 0;
    struct linux_sockaddr_in socket_address = {0};
    st = linux_accept(server->handle, LINUX_SOCKADDR(&socket_address), USZ_REF(sizeof(socket_address)));
    if (!LINUX_SYSCALL_SUCCESS(st))
        return os_socket_errno_code_to_status(SOCK_FN_ACCEPT, USZ(-st));

    client->handle = st;

    address->ipv4 = socket_address.addr;
    address->port = socket_address.port;

    return OS_SOCKET_STATUS_SUCCESS;
}

OS_Socket_Status os_socket_connect(const struct os_socket *client, struct os_socket *server, const struct net_address address)
{
    ssz st = 0;
    struct linux_sockaddr_in socket_address = {0};
    socket_address.addr = address.ipv4;
    socket_address.port = address.port;

    st = linux_connect(client->handle, LINUX_SOCKADDR(&socket_address), sizeof(socket_address));
    server->handle = st;

    return os_socket_errno_code_to_status(SOCK_FN_CONNECT, USZ(-st));
}

OS_Socket_Status os_socket_close(struct os_socket *sock)
{
    ssz st = 0;
    st = linux_close(sock->handle);
    sock->handle = OS_SOCKET_INVALID;
    return os_socket_errno_code_to_status(SOCK_FN_CLOSE, USZ(-st));
}

OS_Socket_Status os_socket_shutdown(const struct os_socket *sock, usz how)
{
    ssz st = 0;
    st = linux_shutdown(sock->handle, how);

    return os_socket_errno_code_to_status(SOCK_FN_SHUTDOWN, USZ(-st));
}

OS_Socket_Status os_socket_receive_data(const struct os_socket *sock, char *buffer, usz size, usz flags)
{
    ssz st = 0;
    st = linux_recv(sock->handle, buffer, size, flags);

    return os_socket_errno_code_to_status(SOCK_FN_RECV, USZ(-st));
}

OS_Socket_Status os_socket_send_data(const struct os_socket *sock, char *buffer, usz size, usz flags)
{
    ssz st = 0;
    st = linux_send(sock->handle, buffer, size, flags);

    return os_socket_errno_code_to_status(SOCK_FN_SEND, USZ(-st));
}

OS_Socket_Status os_socket_errno_code_to_status(usz socket_function, usz errno_val)
{
    if (errno_val == 0)
        return OS_SOCKET_STATUS_SUCCESS;

    switch (socket_function) {
    case SOCK_FN_SOCKET:
        switch (errno_val) {
        case EMFILE: return OS_SOCKET_STATUS_HOST_OUT_OF_SOCKETS;
        case ENOBUFS: return OS_SOCKET_STATUS_HOST_OUT_OF_MEMORY;
        case ENOMEM: return OS_SOCKET_STATUS_HOST_OUT_OF_MEMORY;
        }
        UNREACHABLE();
    case SOCK_FN_BIND:
        switch (errno_val) {
        case EACCES: return OS_SOCKET_STATUS_PROTECTED_ADDRESS;
        case EADDRINUSE: return OS_SOCKET_STATUS_ADDRESS_AND_PORT_IN_USE;
        case EBADF: return OS_SOCKET_STATUS_INVALID_SOCKET;
        case EINVAL: return OS_SOCKET_STATUS_SOCKET_ALREADY_BOUND;
        }
        UNREACHABLE();
    case SOCK_FN_LISTEN:
        switch (errno_val) {
        case EADDRINUSE: return OS_SOCKET_STATUS_ADDRESS_AND_PORT_IN_USE;
        case EBADF: return OS_SOCKET_STATUS_INVALID_SOCKET;
        case ENOTSOCK: return OS_SOCKET_STATUS_INVALID_SOCKET;
        }
        UNREACHABLE();
    case SOCK_FN_ACCEPT:
        switch (errno_val) {
        case EWOULDBLOCK: return OS_SOCKET_STATUS_EMPTY_CONNECTION_QUEUE;
#if !defined(CORE_PLATFORM_LINUX)
        case EAGAIN: return OS_SOCKET_STATUS_EMPTY_CONNECTION_QUEUE;
#endif /* CORE_PLATFORM_LINUX */
        case EBADF: return OS_SOCKET_STATUS_INVALID_SOCKET;
        case ECONNABORTED: return OS_SOCKET_STATUS_CONNECTION_WAS_ABORTED;
        case EINTR: return OS_SOCKET_STATUS_CONNECTION_WAS_INTERRUPTED;
        case EINVAL: return OS_SOCKET_STATUS_SOCKET_IS_NOT_LISTENING;
        case EMFILE: return OS_SOCKET_STATUS_PROCESS_OUT_OF_FDS;
        case ENFILE: return OS_SOCKET_STATUS_HOST_OUT_OF_FDS;
        case ENOBUFS: return OS_SOCKET_STATUS_HOST_OUT_OF_SOCKETS;
        case ENOMEM: return OS_SOCKET_STATUS_HOST_OUT_OF_SOCKETS;
        case ENOTSOCK: return OS_SOCKET_STATUS_INVALID_SOCKET;
        }
        UNREACHABLE();
    case SOCK_FN_CONNECT:
        switch (errno_val) {
        case EPERM: return OS_SOCKET_STATUS_BLOCKED_BY_FIREWALL;
        case EADDRINUSE: return OS_SOCKET_STATUS_ADDRESS_AND_PORT_IN_USE;
        case EALREADY: return OS_SOCKET_STATUS_PREVIOUS_CONNECT_INCOMPLETE;
        case EBADF: return OS_SOCKET_STATUS_INVALID_SOCKET;
        case ECONNREFUSED: return OS_SOCKET_STATUS_SERVICE_NOT_AVAILABLE;
        case EINPROGRESS: TODO("implement EINPROGRESS for connect()"); ABORT();
        case EINTR: return OS_SOCKET_STATUS_CONNECTION_WAS_INTERRUPTED;
        case EISCONN: return OS_SOCKET_STATUS_SOCKET_IS_ALREADY_CONNECTED;
        case ENOTSOCK: return OS_SOCKET_STATUS_INVALID_SOCKET;
        case ETIMEDOUT: return OS_SOCKET_STATUS_CONNECTION_TIMEOUT;
        }
        UNREACHABLE();
    case SOCK_FN_SEND:
        switch (errno_val) {
        case EAGAIN: TODO("implement EAGAIN for send()"); ABORT();
        case EALREADY: TODO("implement EALREADY for send()"); ABORT();
        case EBADF: return OS_SOCKET_STATUS_INVALID_SOCKET;
        case ECONNRESET: return OS_SOCKET_STATUS_PEER_RESET_CONNECTION;
        case EDESTADDRREQ: return OS_SOCKET_STATUS_NO_TARGET_SPECIFIED;
        case EINTR: return OS_SOCKET_STATUS_SEND_GOT_INTERRUPTED_BY_OS;
        case ENOMEM: return OS_SOCKET_STATUS_HOST_OUT_OF_MEMORY;
        case ENOTCONN: return OS_SOCKET_STATUS_SOCKET_IS_NOT_CONNECTED;
        case ENOTSOCK: return OS_SOCKET_STATUS_INVALID_SOCKET;
        }
        UNREACHABLE();
    case SOCK_FN_RECV:
        switch (errno_val) {
        case EBADF: return OS_SOCKET_STATUS_INVALID_SOCKET;
        case ENOTSOCK: return OS_SOCKET_STATUS_INVALID_SOCKET;
        case ECONNREFUSED: return OS_SOCKET_STATUS_PEER_DOWN;
        case EINTR: return OS_SOCKET_STATUS_RECV_GOT_INTERRUPTED_BY_OS;
        case ENOTCONN: return OS_SOCKET_STATUS_SOCKET_IS_NOT_CONNECTED;
        }
        UNREACHABLE();
    case SOCK_FN_SENDTO:
        UNREACHABLE();
    case SOCK_FN_RECVFROM:
        UNREACHABLE();
    case SOCK_FN_SENDMSG:
        UNREACHABLE();
    case SOCK_FN_RECVMSG:
        UNREACHABLE();
    case SOCK_FN_SHUTDOWN:
        UNREACHABLE();
    case SOCK_FN_CLOSE:
        switch (errno_val) {
        case EBADF: return OS_SOCKET_STATUS_INVALID_SOCKET;
        case EINTR: return OS_SOCKET_STATUS_CLOSE_GOT_INTERRUPTED_BY_OS;
        case EIO: return OS_SOCKET_STATUS_INPUT_OUTPUT_ERROR;
        }
        UNREACHABLE();
    case SOCK_FN_GETSOCKOPT:
        UNREACHABLE();
    case SOCK_FN_SETSOCKOPT:
        UNREACHABLE();
    case SOCK_FN_GETSOCKNAME:
        UNREACHABLE();
    case SOCK_FN_GETPEERNAME:
        UNREACHABLE();
    case SOCK_FN_SELECT:
        UNREACHABLE();
    case SOCK_FN_POLL:
        UNREACHABLE();
    case SOCK_FN_EPOLL_CREATE:
        UNREACHABLE();
    case SOCK_FN_EPOLL_CTL:
        UNREACHABLE();
    case SOCK_FN_EPOLL_WAIT:
        UNREACHABLE();
    case SOCK_FN_ACCEPT4:
        UNREACHABLE();
    }

    UNREACHABLE();
}

static char *g_os_socket_status_code_strings[] = {
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_SUCCESS),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_HOST_OUT_OF_FDS),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_PROCESS_OUT_OF_FDS),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_PROCESS_OUT_OF_SOCKETS),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_HOST_OUT_OF_SOCKETS),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_HOST_OUT_OF_MEMORY),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_PROTECTED_ADDRESS),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_ADDRESS_AND_PORT_IN_USE),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_INVALID_SOCKET),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_SOCKET_ALREADY_BOUND),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_EMPTY_CONNECTION_QUEUE),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_CONNECTION_WAS_ABORTED),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_CONNECTION_WAS_INTERRUPTED),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_SOCKET_IS_NOT_LISTENING),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_SOCKET_IS_NOT_CONNECTED),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_BLOCKED_BY_FIREWALL),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_PREVIOUS_CONNECT_INCOMPLETE),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_SERVICE_NOT_AVAILABLE),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_CONNECTION_TIMEOUT),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_RECEIVE_TIMEOUT),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_CLOSE_GOT_INTERRUPTED_BY_OS),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_INPUT_OUTPUT_ERROR),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_PEER_RESET_CONNECTION),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_NO_TARGET_SPECIFIED),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_PEER_DOWN),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_RECV_GOT_INTERRUPTED_BY_OS),
    ENUM_STR_ENTRY(OS_SOCKET_STATUS_UNKNOWN),
};

char *os_socket_string_status(usz code)
{
    if (code > OS_SOCKET_STATUS_UNKNOWN)
        code = OS_SOCKET_STATUS_UNKNOWN;

    return g_os_socket_status_code_strings[code];
}

void os_util_strip_file_extension_ar(const char *file_name, char **dst, struct m_arena *arena)
{
    /* walk backwards until a period is hit */
    usz length = 0;
    char *stripped = NULL;
    length = cstr_length(file_name);

    for (usz i = length - 1; i > 0; i--) {
        if ('.' == file_name[i]) {
            usz stripped_length = length - i;
            stripped = m_arena_alloc(arena, sizeof(*stripped), stripped_length + NULL_TERM_SIZE);
            cstr_copy(*dst, file_name, stripped_length);
            break;
        }
    }

    *dst = stripped;
}

static OS_Stream_Status open_file_stream(struct os_stream *stream, const struct os_file_info info)
{
    os_file_open(&stream->raw.file, info);
    return OS_STREAM_STATUS_SUCCESS;
}

static OS_Stream_Status open_tcp_socket_stream(struct os_stream *stream, const struct os_socket_tcp_listener_info info)
{
    os_socket_create_tcp_listener(&stream->raw.socket, info);
    return OS_STREAM_STATUS_SUCCESS;
}

static OS_Stream_Status open_ipc_socket_stream(struct os_stream *stream, const struct os_socket_ipc_info info)
{
    os_socket_create_ipc_socket(&stream->raw.socket, info);
    return OS_STREAM_STATUS_SUCCESS;
}

static void platform_init()
{
    register_core_signal_handlers();
    os_file_init_proc_io_handles();
    set_coredumps();

}

static void platform_shutdown(void)
{
    reset_coredumps();
}

enum linux_default_io_fds { LINUX_STDIN = 0, LINUX_STDOUT = 1, LINUX_STDERR = 2, };
static void os_file_init_proc_io_handles(void)
{
    g_os_stdin  = (struct os_file) { .handle = LINUX_STDIN };
    g_os_stdout = (struct os_file) { .handle = LINUX_STDOUT };
    g_os_stderr = (struct os_file) { .handle = LINUX_STDERR };

    OS_STDIN    = &g_os_stdin;
    OS_STDOUT   = &g_os_stdout;
    OS_STDERR   = &g_os_stderr;

    ASSERT_RUN_ONCE();
}

static void set_coredumps(void)
{
}

static void reset_coredumps(void)
{
}

static void register_core_signal_handlers(void)
{
    set_linux_signal_handler(LINUX_SIGINT,    core_empty_handler);
}

static void core_empty_handler(int sig_num)
{
    INFO_LOG("caught linux signal: "STR_FMT, linux_signal_str(sig_num));
    WARN_LOG("exiting...");
    ABORT();
}

static void set_linux_signal_handler(int sig_num, void (*handler) (int))
{
    linux_signal(sig_num, handler);
}
