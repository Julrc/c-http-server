#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
	short port;
} server_config;

int loadConfig(server_config *config);
char *loadfile(const char *path);

#endif
