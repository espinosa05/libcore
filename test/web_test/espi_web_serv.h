#ifndef __ESPI_WEB_SERV_H__
#define __ESPI_WEB_SERV_H__

#include <core/types.h>
#include <core/memory.h>
#include <core/os_socket.h>

struct espi_web_server_info {
    struct net_addr server_address;
    usz             connection_queue_length;
};

struct espi_web_server_route {
    usz     destination_type;
    char    *destination;
    char    *route;
};

struct espi_web_server {
    struct os_socket    socket;
    struct m_array      routes;
};

enum espi_web_server_status_codes {
    ESPI_WEB_SERVER_STATUS_SUCCESS = 0,

    ESPI_WEB_SERVER_STATUS_UNKNOWN,
};
typedef usz Espi_Web_Server_Status;

Espi_Web_Server_Status espi_web_server_init(struct espi_web_server *server, const struct espi_web_server_info info);
Espi_Web_Server_Status espi_web_server_add_route(struct espi_web_server *server, const struct espi_web_server_route *route);

Espi_Web_Server_Status espi_web_server_listen(struct espi_web_server *server);
Espi_Web_Server_Status espi_web_server_shutdown(struct espi_web_server *server);

#endif /* __ESPI_WEB_SERV_H__ */
