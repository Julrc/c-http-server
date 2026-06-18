#include "main.h"
#include "tcp.h"
#include "http.h"
#include "route.h"

void hello_handler(http_request *req, http_response *res)
{
	res->status_code = 200;

	if (!res->body) 
	{
		res->body = malloc(64);
	}

	strcpy(res->body, "Hello World!");
	res->body_length = 14;

	add_http_header(res, "Content-Length", "14");
}

int main() 
{
	tcp_server server = {0};

	// server_config config = { .port = 8080, };

	/*
	if (loadConfig(&config) == 0)
	{
		debug_log("Failed to load config, using deafult values.");
	}
	*/

	server_status_e status = bind_tcp_port(&server, 8080);

	if (status != SERVER_OK) 
	{
		debug_log("Server initialization failed");
		exit(EXIT_FAILURE);
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

		http_request req = {0};
		http_response res = {0};

		init_http_response(&res);
		install_route(HTTP_METHOD_GET, "/hello", hello_handler);

		if (read_http_request(client_fd, &req) != HTTP_PARSE_OK) 
		{
			debug_log("Failed to read or parse HTTP request");
			close(client_fd);
			return 0;
		}

		if (parse_http_headers(req.buffer, &req) != HTTP_PARSE_OK) 
		{
			debug_log("Failed to read or parse HTTP headers");
			close(client_fd);
			return 0;
		}

		char sanitized_path[1024] = {0};
		sanitize_path(req.path, sanitized_path, sizeof(sanitized_path));

		if (!handle_request(&req, &res))
		{
			serve_file(sanitized_path, &res);
		}
		send_http_response(client_fd, &res);

		close(client_fd);

	}

	close(server.socket_fd);
	return 0;

}


