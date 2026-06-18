#include "http.h"
#include "config.h"
#include "main.h"
#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>

int loadConfig(server_config *config) 
{
	int status = 0;
	char *configdata = loadfile("config.json");
	if (!configdata)
	{
		debug_log("Failed to laod config, what happened?");
	}
	cJSON *config_json = cJSON_Parse(configdata);
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
	if (!cJSON_IsNumber(port))
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
	config->port = (short)port->valueint;

	end:
		cJSON_Delete(config_json);
		return status;

}


