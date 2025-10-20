#ifndef __CORE_OS_SOCKET_H__
#define __CORE_OS_SOCKET_H__

#include <core/platform.h>
#include <core/cstd.h>

struct net_address {
    u32 ipv4;
    u16 port;
};

#if defined(CORE_PLATFORM_LINUX)
struct os_socket {
    int handle;
};

enum os_socket_shutdown_ops {
    OS_SOCKET_SHUTDOWN_READ = SHUT_RD,
    OS_SOCKET_SHUTDOWN_WRITE = SHUT_WR,
    OS_SOCKET_SHUTDOWN_READ_AND_WRITE = SHUT_RDWR,
};

#elif defined(CORE_PLATFORM_WINDOWS)
struct os_socket {
    SOCKET handle;
};

enum os_socket_shutdown_ops {
    OS_SOCKET_SHUTDOWN_READ = SD_RECEIVE,
    OS_SOCKET_SHUTDOWN_WRITE = SD_REND,
    OS_SOCKET_SHUTDOWN_READ_AND_WRITE = SD_BOTH,
};
#else
# error "platform not supported yet"
#endif /* CORE_PLATFORM_LINUX */

enum os_socket_status_codes {
    OS_SOCKET_STATUS_SUCCESS = 0,
    OS_SOCKET_STATUS_OUT_OF_BOUNDS_READ,
    OS_SOCKET_STATUS_OUT_OF_BOUNDS_WRITE,

#if defined(CORE_PLATFORM_LINUX)
    OS_SOCKET_STATUS_HOST_OUT_OF_FDS,
    OS_SOCKET_STATUS_PROCESS_OUT_OF_FDS,
#elif defined(CORE_PLATFORM_WINDOWS)
/* nothing */
#endif
    OS_SOCKET_STATUS_PROCESS_OUT_OF_SOCKETS,
    OS_SOCKET_STATUS_HOST_OUT_OF_SOCKETS,
    OS_SOCKET_STATUS_HOST_OUT_OF_MEMORY,
    OS_SOCKET_STATUS_PROTECTED_ADDRESS,
    OS_SOCKET_STATUS_ADDRESS_AND_PORT_IN_USE,
    OS_SOCKET_STATUS_INVALID_SOCKET,
    OS_SOCKET_STATUS_SOCKET_ALREADY_BOUND,
    OS_SOCKET_STATUS_EMPTY_CONNECTION_QUEUE,
    OS_SOCKET_STATUS_CONNECTION_WAS_ABORTED,
    OS_SOCKET_STATUS_CONNECTION_WAS_INTERRUPTED,
    OS_SOCKET_STATUS_SOCKET_IS_NOT_LISTENING,
    OS_SOCKET_STATUS_BLOCKED_BY_FIREWALL,
    OS_SOCKET_STATUS_PREVIOUS_CONNECT_INCOMPLETE,
    OS_SOCKET_STATUS_SERVICE_NOT_AVAILABLE,
    OS_SOCKET_STATUS_SOCKET_IS_NOT_CONNECTED,
    OS_SOCKET_STATUS_SOCKET_IS_ALREADY_CONNECTED,
    OS_SOCKET_STATUS_CONNECTION_TIMEOUT,
    OS_SOCKET_STATUS_RECEIVE_TIMEOUT,
    OS_SOCKET_STATUS_CLOSE_GOT_INTERRUPTED_BY_OS,
    OS_SOCKET_STATUS_INPUT_OUTPUT_ERROR,
    OS_SOCKET_STATUS_PEER_RESET_CONNECTION,
    OS_SOCKET_STATUS_SEND_GOT_INTERRUPTED_BY_OS,
    OS_SOCKET_STATUS_NO_TARGET_SPECIFIED,
    OS_SOCKET_STATUS_PEER_DOWN,
    OS_SOCKET_STATUS_RECV_GOT_INTERRUPTED_BY_OS,

    OS_SOCKET_STATUS_UNKNOWN,
    OS_SOCKET_STATUS_COUNT,
};
typedef usz OS_Socket_Status;

#define OS_SOCKET_FAILURE(st) !OS_SOCKET_SUCCESS(st)
#define OS_SOCKET_SUCCESS(st) (st == SU_STATUS_SUCCESS)

struct os_socket_tcp_info {
    struct net_address  *net_addr;
    usz                 queue_length;
};

struct os_socket_ipc_info {
    const char *path;
};

/* wrappers around setsockopt/getsockopt */
OS_Socket_Status os_socket_enable_sockopt(const struct os_socket *sock, sz sock_opt);
OS_Socket_Status os_socket_disableSockopt(const struct os_socket *sock, sz sock_opt);

OS_Socket_Status os_socket_get_sockopt(const struct os_socket *sock, sz sock_opt, sz *value);
OS_Socket_Status os_socket_set_sockobj(const struct os_socket *sock, sz sock_opt, const void *sock_obj, usz size);
OS_Socket_Status os_socket_get_sockobj(const struct os_socket *sock, sz sock_opt, void *sock_obj, usz size);

OS_Socket_Status os_socket_create_tcp_socket(struct os_socket *sock, const struct os_socket_tcp_info info);
OS_Socket_Status os_socket_create_ipc_socket(struct os_socket *sock, const struct os_socket_ipc_info info);

OS_Socket_Status os_socket_accept(const struct os_socket *server, struct os_socket *client, struct net_address *address);
OS_Socket_Status os_socket_connect(const struct os_socket *client, struct os_socket *server, const struct net_address address);

OS_Socket_Status os_socket_close(struct os_socket *sock);
OS_Socket_Status os_socket_shutdown(const struct os_socket *sock, usz how);

OS_Socket_Status os_socket_receive_data(const struct os_socket *sock, char *buffer, const usz buffer_size, usz size, usz flags);
OS_Socket_Status os_socket_send_data(const struct os_socket *sock, char *buffer, const usz buffer_size, usz size, usz flags);

char *os_socket_string_status(usz code);

#endif /* __CORE_OS_SOCKET_H__ */
