#include "arena.h"
#include "http.h"

#define MAX_PATH 1024

typedef struct
{
	U64 hash;
	size_t path_len;
	char *data;
	char path[256];
	size_t data_len;
} cache_entry;

typedef struct
{
	char *data;
	U64 data_len;
} file_content;


void file_cache_init(void);
void file_cache_free(void);
void serve_file(const char *path, http_response *response, Arena *req_arena);
