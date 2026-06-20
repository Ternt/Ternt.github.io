// 2026-06-14

#ifndef ARENA_H
#define ARENA_H

////////////////////////////
//- Arena Type Definitions

// Arena Header Type
typedef struct Arena {
  u32 cap;
  u32 pos;
} Arena;

// Temp Type
typedef struct Temp {
  Arena *arena;
  u32 pos;
} Temp;

////////////////////////////
//- Arena Constants

#define ARENA_HEADER_SIZE Clamp(32, sizeof(Arena), 64)
#define ARENA_DEFAULT_CAP MB(32)

////////////////////////////
//- Arena Functions

// core arena operations
static Arena *ArenaAlloc(u32 cap);
static void   ArenaRelease(Arena *arena);
static void  *ArenaPush(Arena *arena, u32 size, u32 align);
static void   ArenaPopTo(Arena *arena, u32 pos);
static void   ArenaPop(Arena *arena, u32 amt);
static void   ArenaClear(Arena *arena);
static Temp   TempBegin(Arena *arena);
static void   TempEnd(Temp temp);

// function-like helper macros
#define PushArrayAligned(a,T,c,align) ArenaPush(a,(c)*sizeof(T),align)
#define PushArray(a,T,c) PushArrayAligned(a,T,c,Max(8,AlignOf(T)))
#define PushStructAligned(a,T,align) ArenaPush(a,sizeof(T),align)
#define PushStruct(a,T) PushStructAligned(a,T,Max(8,AlignOf(T)))

#endif // ARENA_H
