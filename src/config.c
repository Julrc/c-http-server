#include "config.h"
#include "main.h"

#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>

char *loadfile(const char *path)
{
	FILE *file = fopen(path, "rb");

	if (!file)
	{
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	size_t file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char *file_content = malloc(file_size + 1);

	if (!file_content)
	{
		perror("Failed to allocate memory for file content");
		fclose(file);
		exit(EXIT_FAILURE);
	}

	fread(file_content, 1, file_size, file);
	fclose(file);
	file_content[file_size] = '\0';

	return file_content;
}

int loadConfig(server_config *config) 
{
	int status = 1;
	char *configdata = loadfile("config.json");

	if (!configdata)
	{
		debug_log("Failed to laod config, what happened?");
	}
	cJSON *config_json = cJSON_Parse(configdata);
	free(configdata);
	if (config_json == NULL)
	{
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL)
		{
			fprintf(stderr, "Error before: %s\n", error_ptr);
		}
		status = 0;
		goto end;
	}
	cJSON *port = cJSON_GetObjectItemCaseSensitive(config_json, "portnumber");
	cJSON *thread_count = cJSON_GetObjectItemCaseSensitive(config_json, "thread_count");
	if (!cJSON_IsNumber(port) && !cJSON_IsNumber(thread_count))
	{
		status = 0;
		goto end;
	}

	if (port->valueint > 65535 || port->valueint < 0)
	{
		debug_log("Invalid port number specified in config");
		status = 0;
		goto end;
	}

	if (thread_count->valueint < 1 || thread_count->valueint > 64)
	{
		debug_log("Invalid number thread count specified in config");
		status = 0;
		goto end;

	}

	config->port = (short)port->valueint;
	config->thread_count = (short)thread_count->valueint;

	end:
		cJSON_Delete(config_json);
		return status;
}


