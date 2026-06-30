#include <stdlib.h>

#include "arena.h"

Arena *ArenaAlloc(void)
{
	Arena *arena= malloc(sizeof(Arena));
	if (arena == NULL)
	{
		return NULL;
	}

	arena->mem = malloc(4 * GiB);
	arena->offset= 0;
	arena->capacity = 4 * GiB;

	if (arena->mem == NULL)
	{
		free(arena);
		return NULL;
	}

	return arena;
}

void ArenaRelease(Arena *arena)
{
	free(arena->mem);
	free(arena);
}

void *ArenaPushNoZero(Arena *arena, U64 size)
{
	if (arena == NULL)
	{
		exit(EXIT_FAILURE);
	}

	if (arena->offset + size > arena->capacity)
	{
		return NULL;
	}

	void *ret_mem = arena->mem + arena->offset;
	arena->offset += size;

	return ret_mem;
}

void ArenaPop(Arena *arena, U64 size)
{
	arena->offset -= size;
}


void ArenaClear(Arena *arena)
{
	arena->offset = 0;
}

