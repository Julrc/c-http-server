#include "config.h"
#include "http.h"
#include "main.h"
#include "route.h"
#include "tcp.h"
#include "worker.h"

#include "thread_pool.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void hello_handler(http_request *req, http_response *res)
{
	res->status_code = 200;

	if (!res->body) 
	{
		res->body = malloc(64);
	}

	strcpy(res->body, "Hello World!\n");
	res->body_length = 13;

	add_http_header(res, "Content-Length", "13");
	add_http_header(res, "Connection", "close");
}

int main(void) 
{
	tcp_server server = {0};

	server_config config = { .port = 8080, .thread_count = 8};

	if (loadConfig(&config) == 0)
	{
		debug_log("Failed to load config, using default values.");
	}

	server_status_e status = bind_tcp_port(&server, config.port);

	if (status != SERVER_OK) 
	{
		debug_log("Server initialization failed");
		exit(EXIT_FAILURE);
	}

	install_route(HTTP_METHOD_GET, "/hello", hello_handler);

	thread_pool_t pool;
	if (thread_pool_init(&pool, config.thread_count, 100) != 0) {
		printf("Failed to initialize pool\n");
		exit(-1);
	}

	for (;;) 
	{
		int client_fd = accept_client(server.socket_fd);
		if (client_fd == -1) 
		{
			debug_log("Failed to accept client connections");
			close(server.socket_fd);
			exit(EXIT_FAILURE);
		}

		debug_log("Client connected");

		int *client_fd_p = malloc(sizeof(int));
		if (!client_fd_p)
		{
			debug_log("Failed to allocate memory");
			close(client_fd);
			continue;
		}
		*client_fd_p = client_fd;
		thread_pool_add_task(&pool, work_task_th, (void *)client_fd_p);
	}

	close(server.socket_fd);

	thread_pool_destroy(&pool);

	return 0;

}
