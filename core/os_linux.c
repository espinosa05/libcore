#include <core/os.h>
#include <core/os_dynamic_library.h>
#include <core/os_streams.h>
#include <core/os_file.h>
#include <core/os_socket.h>

#include <core/utils.h>
#include <core/strings.h>
#include <core/types.h>
#include <core/memory.h>
#include <core/cstd.h>
#include <core/linux.h>
#include <core/vulkan.h>


#define OS_LINUX_SYSCALL_SUCCESS(rc) (rc >= 0)

/* static function declaration start */
static OS_Stream_Status open_file_stream(struct os_stream *stream, const struct os_file_info info);
static OS_Stream_Status open_tcp_socket_stream(struct os_stream *stream, const struct os_socket_tcp_info info);
static OS_Stream_Status open_ipc_socket_stream(struct os_stream *stream, const struct os_socket_ipc_info info);
static OS_Socket_Status os_socket_errno_code_to_status(sz status_code, usz socket_function, usz errno_val);
static void os_file_init_proc_io_handles(void);
static void set_coredumps(void);
static void reset_coredumps(void) __FUNC_ATTR_DESTRUCTOR__;
static void platform_init() __FUNC_ATTR_CONSTRUCTOR__;
/* static function declaration end */

/* global data start */
struct os_file *OS_STDIN;
struct os_file *OS_STDOUT;
struct os_file *OS_STDERR;

static struct os_file __OS_StdIn;
static struct os_file __OS_StdOut;
static struct os_file __OS_StdErr;

static struct rlimit initial_coredump_count = {0};
/* global data end */

void os_open_library(struct os_library *lib, const struct os_library_info info)
{
    ASSERT(info.name, "No library name passed!");

    char *path = info.path;
    if (!path)
        path = "./";

    char *actual_path_str = NULL;
    {
        struct str_builder whole_path = {0};
        str_builder_init(&whole_path, os_get_max_path_length(info.path));
        str_builder_append(&whole_path, info.path);
        str_builder_append(&whole_path, info.name);
        str_builder_append(&whole_path, ".so");
        str_builder_to_cstr_alloc(&whole_path, &actual_path_str);
        str_builder_delete(whole_path);
    }

    lib->handle = dlopen(actual_path_str, RTLD_LAZY);
    ASSERT_RT(lib->handle, "failed to open dynamic library \"%s\" %s", actual_path_str, dlerror());

    m_free(actual_path_str);
}

void os_load_library_symbol(struct os_library *lib, void **dst, const char *symbol)
{
   *dst = dlsym(lib, symbol);
   ASSERT_RT(*dst, "failed to load symbol %s: %s", symbol, dlerror());
}

void os_close_library(struct os_library lib)
{
    int st = dlclose(lib.handle);
    ASSERT_RT(st == 0, "failed to close dynamic library: %s", dlerror());
}

enum threadStatus { THREAD_FAILED = -1, THREAD_CREATED = 0, };
OS_Thread_Status os_thread_spawn(struct os_thread *thr, void (*func) (void *), void *arg)
{
    struct clone_args clone_args = {0};
    struct rlimit stack_size = {0};
    void *stack_buffer = NULL;
    OS_Thread_Status ret = OS_THREAD_STATUS_FAILURE;

    /* getrlimit does not return useful errors */
    getrlimit(RLIMIT_STACK, &stack_size);
    stack_buffer = mmap(NULL, stack_size.rlim_cur, PROT_READ | PROT_WRITE,
                        MAP_GROWSDOWN | MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);

    ASSERT_RT(stack_buffer != MAP_FAILED, "failed to allocate stack for new process");

    clone_args.flags         = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND;
    clone_args.exit_signal   = SIGCHLD;
    clone_args.stack         = (u64)stack_buffer;
    clone_args.stack_size    = stack_size.rlim_cur;

    thr->pid = syscall(SYS_clone3, &clone_args, sizeof(clone_args));

    switch (thr->pid) {
    case THREAD_FAILED:
        ret = OS_THREAD_STATUS_FAILED_TO_CREATE;
        break;
    case THREAD_CREATED:
        INFO_LOG("thread started with PID: %d", thr->pid);
        func(arg);
        ret = OS_THREAD_STATUS_SUCCESS;
        break;
    }
    return ret;
}

OS_Thread_Status os_thread_join(struct os_thread *thr, void *ret)
{
    TODO("implement OS_ThreadJoin");
    UNUSED(ret);

    int status = 0;
    waitpid(thr->pid, &status, 0);

    return OS_THREAD_STATUS_SUCCESS;;
}

void os_time_get_current(struct os_time *time)
{
    ASSERT_RT(0 == clock_gettime(CLOCK_REALTIME, &time->time_val),
              "couldn't get system time: "STR_FMT, get_errno_str(errno));
}

void os_time_get_monotonic(struct os_time *time)
{
    ASSERT_RT(0 == clock_gettime(CLOCK_MONOTONIC, &time->time_val),
              "couldn't measure time start: "STR_FMT, get_errno_str(errno));
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
    return ((f64)os_time_get_usec(time))/1000;
}

usz os_time_get_sec(const struct os_time time)
{
    return ((usz)os_time_get_msec(time))/1000;
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

OS_Stream_Status os_stream_read(struct os_stream *stream, void *buffer, const usz buffer_size, const usz size)
{
    switch (stream->type) {
    case OS_STREAM_TYPE_IPC:
    case OS_STREAM_TYPE_NETWORK:
        os_socket_receive_data(&stream->raw.socket, buffer, buffer_size, size, 0);
        break;
    case OS_STREAM_TYPE_FILE:
        os_file_read(&stream->raw.file, buffer, buffer_size, size);
        break;
    case OS_STREAM_TYPE_BYTE:
        m_buffer_read(&stream->raw.byte_buffer, buffer, buffer_size, size);
        break;
    }

    return OS_STREAM_STATUS_SUCCESS;
}

OS_Stream_Status os_stream_write(struct os_stream *stream, void *buffer, const usz buffer_size, const usz size)
{
    switch (stream->type) {
    case OS_STREAM_TYPE_IPC:
    case OS_STREAM_TYPE_NETWORK:
        os_socket_send_data(&stream->raw.socket, buffer, buffer_size, size, 0);
        break;
    case OS_STREAM_TYPE_FILE:
        os_file_write(&stream->raw.file, buffer, buffer_size, size);
        break;
    case OS_STREAM_TYPE_BYTE:
        m_buffer_write(&stream->raw.byte_buffer, buffer, buffer_size, size);
        break;
    }

    return OS_STREAM_STATUS_SUCCESS;
}

OS_Stream_Status os_stream_printf(struct os_stream *stream, const char *fmt, ...)
{
    char *buffer = NULL;
    va_list args;
    usz mesage_length;

    va_start(args, fmt);
    cstr_format_alloc_variadic(&buffer, fmt, args, &mesage_length);
    os_stream_write(stream, buffer, mesage_length, mesage_length);

    m_free(buffer);

    return OS_STREAM_STATUS_SUCCESS;
}


void os_file_open(struct os_file *f, const struct os_file_info info)
{
    TODO("error handling");
    f->handle = open(info.path, info.perm);
    ASSERT_RT(OS_LINUX_SYSCALL_SUCCESS(f->handle), "syscall open failed!");
}

void os_file_create(struct os_file *f, const struct os_file_info info)
{
    TODO("error handling");
    u32 fd_perm = info.perm | O_CREAT; /* the user does not need to know this happens */
    f->handle = open(info.path, fd_perm, S_IWUSR);
    ASSERT_RT(OS_LINUX_SYSCALL_SUCCESS(f->handle), "open syscall failed!");
}

void os_file_read(const struct os_file *f, void *buffer, const usz buffer_size, const usz bytes)
{
    TODO("error handling");
    ASSERT(bytes <= buffer_size, "out of bounds read!");
    sz st = read(f->handle, buffer, bytes);
    ASSERT_RT(OS_LINUX_SYSCALL_SUCCESS(st), "read syscall failed!");
}

void os_file_write(const struct os_file *f, const void *buffer, const usz buffer_size, const usz bytes)
{
    ASSERT(bytes <= buffer_size, "out of bounds write!");
    sz st = write(f->handle, buffer, bytes);
    ASSERT_RT(OS_LINUX_SYSCALL_SUCCESS(st), "syscall write failed!");
}

void os_file_printf(const struct os_file *f, const char *fmt, ...)
{
    char *msg;
    va_list args;
    usz msg_length = 0;

    va_start(args, fmt);
    cstr_format_alloc_variadic(&msg, fmt, args, &msg_length);
    os_file_write(f, msg, msg_length, msg_length);
    m_free(msg);
}

void os_file_close(struct os_file *f)
{
    TODO("error handling");
    sz st = close(f->handle);
    ASSERT_RT(OS_LINUX_SYSCALL_SUCCESS(st), "syscall close failed!");
    f->handle = OS_FILE_INVALID;
}

void os_file_flush(const struct os_file *f)
{
    IMPL();
    UNUSED(f);
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
    sz st = setsockopt(sock->handle, SOL_SOCKET, sock_opt, SOCKOPT_ENABLE, sizeof(int));
    return os_socket_errno_code_to_status(st, SOCK_FN_SETSOCKOPT, errno);
}

OS_Socket_Status os_socket_disable_sock_opt(const struct os_socket *sock, sz sock_opt)
{
    sz st = setsockopt(sock->handle, SOL_SOCKET, sock_opt, SOCKOPT_DISABLE, (socklen_t) {sizeof(int)});
    return os_socket_errno_code_to_status(st, SOCK_FN_SETSOCKOPT, errno);
}

OS_Socket_Status os_socket_get_sockopt(const struct os_socket *sock, sz sock_opt, sz *value)
{
    sz st = getsockopt(sock->handle, SOL_SOCKET, sock_opt, value, &(socklen_t) {sizeof(value)});
    return os_socket_errno_code_to_status(st, SOCK_FN_GETSOCKOPT, errno);
}

OS_Socket_Status os_socket_set_sockobj(const struct os_socket *sock, sz sock_opt, const void *sock_obj, usz size)
{
    sz st = setsockopt(sock->handle, SOL_SOCKET, sock_opt, sock_obj, size);
    return os_socket_errno_code_to_status(st, SOCK_FN_SETSOCKOPT, errno);
}

OS_Socket_Status os_socket_get_sockobj(const struct os_socket *sock, sz sock_opt, void *sock_obj, usz size)
{
    sz st = getsockopt(sock->handle, SOL_SOCKET, sock_opt, sock_obj, &(socklen_t) {size});
    return os_socket_errno_code_to_status(st, SOCK_FN_GETSOCKOPT, errno);
}

OS_Socket_Status os_socket_create_tcp_socket(struct os_socket *sock, const struct os_socket_tcp_info info)
{
    CHECK_NULL(info.net_addr);

    struct sockaddr_in socket_address = {0};
    sz st = 0;

    st = socket(AF_INET, SOCK_STREAM, 0);
    if (!OS_LINUX_SYSCALL_SUCCESS(st))
        return os_socket_errno_code_to_status(st, SOCK_FN_SOCKET, errno);

    sock->handle = st;
    socket_address.sin_family        = AF_INET;
    socket_address.sin_addr.s_addr   = info.net_addr->ipv4;
    socket_address.sin_port          = info.net_addr->port;

    st = bind(sock->handle, (struct sockaddr *)&socket_address, sizeof(socket_address));
    if (!OS_LINUX_SYSCALL_SUCCESS(st))
        return os_socket_errno_code_to_status(st, SOCK_FN_BIND, errno);

    st = listen(sock->handle, info.queue_length);

    return os_socket_errno_code_to_status(st, SOCK_FN_LISTEN, errno);
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
    sz st = 0;
    struct sockaddr_in socket_address = {0};
    st = accept(server->handle, (struct sockaddr *)&socket_address, &(socklen_t) {sizeof(socket_address)});
    if (!OS_LINUX_SYSCALL_SUCCESS(st))
        return os_socket_errno_code_to_status(st, SOCK_FN_ACCEPT, errno);

    client->handle = st;
    address->ipv4 = socket_address.sin_addr.s_addr;
    address->port = socket_address.sin_port;

    return OS_SOCKET_STATUS_SUCCESS;
}

OS_Socket_Status os_socket_connect(const struct os_socket *client, struct os_socket *server, const struct net_address address)
{
    usz st = 0;
    struct sockaddr_in socket_address = {
        .sin_addr.s_addr    = address.ipv4,
        .sin_port           = address.port,
    };

    st = connect(client->handle, (struct sockaddr *)&socket_address, sizeof(socket_address));
    server->handle = st;

    return os_socket_errno_code_to_status(st, SOCK_FN_CONNECT, errno);
}

OS_Socket_Status os_socket_close(struct os_socket *sock)
{
    sz st = 0;
    st = close(sock->handle);
    sock->handle = OS_SOCKET_INVALID;
    return os_socket_errno_code_to_status(st, SOCK_FN_CLOSE, errno);
}

OS_Socket_Status os_socket_shutdown(const struct os_socket *sock, usz how)
{
    sz st = 0;
    st = shutdown(sock->handle, how);

    return os_socket_errno_code_to_status(st, SOCK_FN_SHUTDOWN, errno);
}

OS_Socket_Status os_socket_receive_data(const struct os_socket *sock, char *buffer, const usz buffer_size, usz size, usz flags)
{
    sz st = 0;

    if (size > buffer_size) {
        return OS_SOCKET_STATUS_OUT_OF_BOUNDS_READ;
    }
    st = recv(sock->handle, buffer, size, flags);

    return os_socket_errno_code_to_status(st, SOCK_FN_RECV, errno);
}

OS_Socket_Status os_socket_send_data(const struct os_socket *sock, char *buffer, const usz buffer_size, usz size, usz flags)
{
    sz st = 0;

    if (size > buffer_size) {
        return OS_SOCKET_STATUS_OUT_OF_BOUNDS_WRITE;
    }
    st = send(sock->handle, buffer, size, flags);

    return os_socket_errno_code_to_status(st, SOCK_FN_SEND, errno);
}


/* big ugly function... don't blame me. Blame horrily outdated POSIX errors */
static OS_Socket_Status os_socket_errno_code_to_status(sz status_code, usz socket_function, usz errno_val)
{
    if (status_code == 0)
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

static OS_Stream_Status open_file_stream(struct os_stream *stream, const struct os_file_info info)
{
    os_file_open(&stream->raw.file, info);
    return OS_STREAM_STATUS_SUCCESS;
}

static OS_Stream_Status open_tcp_socket_stream(struct os_stream *stream, const struct os_socket_tcp_info info)
{
    os_socket_create_tcp_socket(&stream->raw.socket, info);
    return OS_STREAM_STATUS_SUCCESS;
}

static OS_Stream_Status open_ipc_socket_stream(struct os_stream *stream, const struct os_socket_ipc_info info)
{
    os_socket_create_ipc_socket(&stream->raw.socket, info);
    return OS_STREAM_STATUS_SUCCESS;
}

static void platform_init()
{
    os_file_init_proc_io_handles();
    set_coredumps();
}

enum linux_default_io_fds { OS_LINUX_STDIN = STDIN_FILENO, OS_LINUX_STDOUT = STDOUT_FILENO, OS_LINUX_STDERR = STDERR_FILENO, };
static void os_file_init_proc_io_handles(void)
{
    __OS_StdIn  = (struct os_file) { .handle = OS_LINUX_STDIN };
    __OS_StdOut = (struct os_file) { .handle = OS_LINUX_STDOUT };
    __OS_StdErr = (struct os_file) { .handle = OS_LINUX_STDERR };

    OS_STDIN    = &__OS_StdIn;
    OS_STDOUT   = &__OS_StdOut;
    OS_STDERR   = &__OS_StdErr;

    ASSERT_RUN_ONCE();
}

static void set_coredumps(void)
{
    ASSERT_RUN_ONCE();

    ASSERT_RT(OS_LINUX_SYSCALL_SUCCESS(getrlimit(RLIMIT_CORE, &initial_coredump_count)),
              "getrlimit failed!: %s", get_errno_str(errno));
}

static void reset_coredumps(void)
{
    ASSERT_RUN_ONCE();
}
