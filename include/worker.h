#ifndef WORKER_H
#define WORKER_H

#include "arena.h"

typedef struct {
	int *fd;
	Arena *request_arena;
} thread_args_t;

void work_task_th(void *vargs, void *ctx);

#endif
