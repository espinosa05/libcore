#include "espi_web_serv.h"

Espi_Web_Server_Status espi_web_server_init(struct espi_web_server *server, const struct espi_web_server_info info)
{
    usz st = OS_SOCKET_STATUS_SUCCESS;
    struct os_socket_tcp_listener_info server_socket_info = {
        .net_addr       = info.server_address,
        .queue_length   = info.connection_queue_length,
        .blocking       = FALSE,
    };
    st = os_socket_create_tcp_listener(server->server_socket, server_socket_info);
    if (OS_SOCKET_FAILURE(st)) {
        return ESPI_WEB_SERVER_STATUS_FAILED_TO_CREATE_LISTENER;
    }

    return ESPI_WEB_SERVER_STATUS_SUCCESS;
}

Espi_Web_Server_Status espi_web_server_add_route(struct espi_web_server *server, const struct espi_web_server_route *route)
{

    return ESPI_WEB_SERVER_STATUS_SUCCESS;
}


Espi_Web_Server_Status espi_web_server_listen(struct espi_web_server *server)
{
    return ESPI_WEB_SERVER_STATUS_SUCCESS;
}

Espi_Web_Server_Status espi_web_server_shutdown(struct espi_web_server *server)
{
    return ESPI_WEB_SERVER_STATUS_SUCCESS;
}
