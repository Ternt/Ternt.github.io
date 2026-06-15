// 2026-06-14

#ifndef HELPERS_H
#define HELPERS_H

////////////////////////////
//- Helper CLib Includes

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

////////////////////////////
//- Helper Type Definitions

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef int8_t    s8;
typedef int16_t   s16;
typedef int32_t   s32;
typedef int64_t   s64;
typedef s8        b8;
typedef s16       b16;
typedef s32       b32;
typedef s64       b64;
typedef float     f32;
typedef double    f64;
typedef size_t    usize;
typedef uintptr_t uptr;

////////////////////////////
//- Helper Intrinsics

#if defined(_MSC_VER)
# define AlignOf(T) __alignof(T)
#elif defined(__clang__) 
# define AlignOf(T) __alignof(T)
#elif defined(__GNUC__)
# define AlignOf(T) __alignof__(T)
#else
# error AlignOf not available for this compiler.
#endif

#if defined(_MSC_VER)
# define force_inline __forceinline
#elif defined(__clang__) || defined(__GNUC__)
# define force_inline __attribute__((always_inline))
#else
# error force_inline not available for this compiler.
#endif

////////////////////////////
//- Helper Macros

#define KB(x) ((u32)(x)<<10)
#define MB(x) ((u32)(x)<<20)
#define GB(x) ((u32)(x)<<30)

#define Max(a,b)       (((a)>(b))?(a):(b))
#define Min(a,b)       (((a)<(b))?(a):(b))
#define ClampTop(a,b)  Min(a,b)
#define ClampBot(a,b)  Max(a,b)
#define Clamp(a,x,b)   (((x)<(a))?(a):(((x)>(b))?(b):(x)))
#define AlignPow2(x,a) (((x)+(a)-1)&(~((a)-1)))
#define OffsetPtr(p,o) (void*)((uptr)(p) + (uptr)(o))

#define MemoryCopy(d,s,c) memmove(d,s,c)

////////////////////////////
//- Helper Global Constants

#define max_u8  (u8)0xff
#define max_u16 (u16)0xffff
#define max_u32 (u32)0xffffffff
#define max_u64 (u64)0xffffffffffffffffllu

////////////////////////////
//- Helper Functions

static inline Vector2 world_ToScreen(Vector2 v);

#endif // HELPERS_H
