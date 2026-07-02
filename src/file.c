#include "file.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

static Arena *cache_arena;
static cache_entry cache[128];
static int cache_sz = 0;
static pthread_mutex_t cache_lock = PTHREAD_MUTEX_INITIALIZER;

void file_cache_init(void)
{
	cache_arena = ArenaAlloc();
}

void file_cache_free(void)
{
	ArenaRelease(cache_arena);
}

U64 hash_fn(const char *s, size_t len)
{
	U64 hsh = 1469598103934665603ULL;
	for (size_t i = 0; i < len; ++i)
	{
		hsh ^= (unsigned char)s[i];
		hsh *= 1099511628211ULL;
	}
	return hsh;
}

cache_entry *find_file(const char *path, size_t len, U64 hsh)
{
	for (int i = 0; i < cache_sz; ++i)
	{
		if (cache[i].hash == hsh && cache[i].path_len == len && memcmp(cache[i].path, path, len)== 0)
		{
			return &cache[i];

		}
	}
	return NULL;
}

void serve_file(const char *path, http_response *response, Arena *req_arena)
{

	// find file in file_cache array ()
	U64 len = strlen(path);
	U64 hsh = hash_fn(path, len);

	pthread_mutex_lock(&cache_lock);
	cache_entry *entry_match = find_file(path, len, hsh);

	if (entry_match)
	{
		response->body = entry_match->data;
		response->body_length = entry_match->data_len;
		pthread_mutex_unlock(&cache_lock);
	}

	if (entry_match == NULL)
	{
		pthread_mutex_unlock(&cache_lock);


		FILE *file = fopen(path, "rb+");
		if (!file)
		{
			// if function calls NULL, get 404.html
			response->status_code = 404;
			strncpy(response->reason_phrase, "Not found", sizeof(response->reason_phrase) - 1);
			serve_file("./www/404.html", response, req_arena);
			return ;
		}
		fseek(file, 0, SEEK_END);
		size_t file_size = ftell(file);
		fseek(file ,0, SEEK_SET);

		pthread_mutex_lock(&cache_lock);
		char *file_content = ArenaPushNoZero(cache_arena, file_size + 1);
		pthread_mutex_unlock(&cache_lock);

		if (!file_content)
		{
			perror("Failed to allocate memory for file content");
			fclose(file);
			exit(EXIT_FAILURE);
		}
		fread(file_content, 1, file_size, file);
		fclose(file);
		file_content[file_size] = '\0';

		pthread_mutex_lock(&cache_lock);
		if (!find_file(path, len, hsh))
		{
			cache_entry new_entry = { .hash = hsh, .path_len = len , .data = file_content, .data_len = file_size};
			memcpy(new_entry.path, path, len + 1);
			cache[(cache_sz++) % 128] = new_entry;
		}
		pthread_mutex_unlock(&cache_lock);
		response->body = file_content;
		response->body_length = file_size;
	}

	if (strstr(path, ".html"))
	{
		add_http_header(response, "Content-Type", "text/html");
	}
	else if (strstr(path, ".css"))
	{
		add_http_header(response, "Content-Type", "text/css");
	}
	else if (strstr(path, ".js"))
	{
		add_http_header(response, "Content-Type", "application/javascript");
	}
	else if (strstr(path, ".png"))
	{
		add_http_header(response, "Content-Type", "image/png");
	}
	else
	{
		add_http_header(response, "Content-Type", "application/octet-stream");
	}
	char content_length[32];
	snprintf(content_length, sizeof(content_length), "%zu", response->body_length);
	add_http_header(response, "Content-Length", content_length);
	add_http_header(response, "Connection", "close");
}


