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

////////////////////////////
//- Arena Constants

#define ARENA_HEADER_SIZE Clamp(32, sizeof(Arena), 64)
#define ARENA_DEFAULT_CAP MB(2)

////////////////////////////
//- Arena Functions

// core arena operations
static Arena *arena_alloc(u32 cap);
static void   arena_release(Arena *arena);
static void  *arena_push(Arena *arena, u32 size, u32 align);
static void   arena_popTo(Arena *arena, u32 pos);
static void   arena_pop(Arena *arena, u32 amt);
static void   arena_clear(Arena* arena);

// function-like helper macros
#define push_arrayAligned(a,T,c,align) arena_push(a,(c)*sizeof(T),align)
#define push_array(a,T,c) push_arrayAligned(a,T,c,Max(8,AlignOf(T)))
#define push_structAligned(a,T,align) arena_push(a,sizeof(T),align)
#define push_struct(a,T) push_arrayAligned(a,T,Max(8,AlignOf(T)))

#endif // ARENA_H
