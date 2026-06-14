// 2026-06-14

#ifndef HELPERS_H
#define HELPERS_H

////////////////////////////
//- Helper CLib Includes

#include <stdlib.h>
#include <stdint.h>

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

////////////////////////////
//- Helper Intrinsics

#define AlignOf(T) __alignof__(T)

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
#define OffsetPtr(p,o) (void*)((usize)(p) + (usize)(o))

////////////////////////////
//- Helper Global Constants

static const u8  max_u8  = 0xff;
static const u16 max_u16 = 0xffff;
static const u32 max_u32 = 0xffffffff;
static const u64 max_u64 = 0xffffffffffffffffllu;

#endif // HELPERS_H
