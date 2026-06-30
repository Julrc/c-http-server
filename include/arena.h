#ifndef ARENA_H
#define ARENA_H

#include <stdint.h>

#define GiB (1ULL << 30)

typedef uint8_t U8;
typedef uint64_t U64;

typedef struct{
	U8 *mem;
	U64 offset;
	U64 capacity;
} Arena;

Arena *ArenaAlloc(void);
void ArenaRelease(Arena *arena);

void *ArenaPushNoZero(Arena *arena, U64 size);

void ArenaPop(Arena *arena, U64 size);

void ArenaClear(Arena *arena);

#endif
