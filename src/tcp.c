#include "tcp.h"
#include "main.h"

#include <string.h>
#include <unistd.h>

server_status_e bind_tcp_port(tcp_server *server, int port) 
{
	server->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server->socket_fd == -1)
	{
		debug_log("Socket creation failed");
		return SERVER_SOCKET_ERROR;
	}

	memset(&server->address, 0, sizeof(server->address));
	server->address.sin_family = AF_INET;
	server->address.sin_addr.s_addr = INADDR_ANY;
	server->address.sin_port = htons(port);

	int opt = 1;

	if (setsockopt(server->socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		perror("setsockopt");
	}

	if (bind(server->socket_fd, (struct sockaddr *)&server->address, sizeof(server->address)) < 0)
	{
		perror("bind");
		debug_log("Bind failed");
		close(server->socket_fd);
		return SERVER_BIND_ERROR;
	}

	if (listen(server->socket_fd, 511) < 0)
	{
		debug_log("Listen failed");
		close(server->socket_fd);
		return SERVER_LISTEN_ERROR;
	}

	debug_log("Server bound and listening");
	return SERVER_OK;
}

int accept_client(int server_fd) 
{
	struct sockaddr_in client_address = {0};
	socklen_t client_len = sizeof(client_address);

	int client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_len);
	if (client_fd < 0) 
	{
		debug_log("Accept failed");
		return -1;
	}
	return client_fd;
}
