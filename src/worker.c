#include "worker.h"
#include "http.h"
#include "main.h"

#include <stdlib.h>
#include <unistd.h>

void work_task_th(void *vargs)
{
	int client_fd = *(int *)vargs;
	free(vargs);
	
	http_request req = {0};
	http_response res = {0};

	init_http_response(&res);

	if (read_http_request(client_fd, &req) != HTTP_PARSE_OK) 
	{
		debug_log("failed to read or parse http request");
		close(client_fd);
		return;
	}

	if (parse_http_headers(req.buffer, &req) != HTTP_PARSE_OK) 
	{
		debug_log("failed to read or parse http headers");
		close(client_fd);
		return;
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

