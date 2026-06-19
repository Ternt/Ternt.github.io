// 2026-06-14

#ifndef HELPERS_H
#define HELPERS_H

////////////////////////////
//- Helper CLib Includes

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

////////////////////////////
//- Helper Keywords

#define null (0)
#define zero_struct {0}

#ifndef true
#define true 1
#endif
#ifndef false 
#define false 0
#endif

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

// Debug Break Intrinsic
#if defined(_MSC_VER)
# define Trap() __debugbreak()
#elif defined(__clang__) || defined(__GNUC__)
# define Trap() __builtin_trap()
#else
# error Trap not available for this compiler.
#endif

// Assertions
#define AssertAlways(x) do{if(!(x)){ Trap(); }}while(0);
#if DEBUG
#define Assert(x) AssertAlways(x)
#else
#define Assert(x) (void)(x)
#endif
#define StaticAssert(C,ID) global U8_t Glue(ID, __LINE__)[(!!(C))*2-1]

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

#define ArrayCount(a)  (sizeof(a)/sizeof(a[0]))
#define InRange(x,a,b) (((x) >= (a))&&((x) <= (b)))

#define MemoryCopy(d,s,c)   memmove(d,s,c)
#define MemoryMatch(a,b,sz) (memcmp(a,b,sz)==0)

#define MemoryZero(p,sz)    memset(p,0,sz)
#define MemoryZeroArray(a)  MemoryZero(a,sizeof(a))
#define MemoryZeroStruct(s) MemoryZero(s,sizeof(*(s)))

////////////////////////////////
//- Linked List Builder Macros

#define CheckNull(p) ((p)==0)
#define SetNull(p) ((p)=0)

#define DLLInsert_NPZ(f,l,p,n,next,prev,zchk,zset) \
(zchk(f) ? (((f) = (l) = (n)), zset((n)->next), zset((n)->prev)) :\
zchk(p) ? (zset((n)->prev), (n)->next = (f), (zchk(f) ? (0) : ((f)->prev = (n))), (f) = (n)) :\
((zchk((p)->next) ? (0) : (((p)->next->prev) = (n))), (n)->next = (p)->next, (n)->prev = (p), (p)->next = (n),\
((p) == (l) ? (l) = (n) : (0))))
#define DLLPushBack_NPZ(f,l,n,next,prev,zchk,zset) DLLInsert_NPZ(f,l,l,n,next,prev,zchk,zset)
#define DLLRemove_NPZ(f,l,n,next,prev,zchk,zset) (((f)==(n))?\
((f)=(f)->next, (zchk(f) ? (zset(l)) : zset((f)->prev))):\
((l)==(n))?\
((l)=(l)->prev, (zchk(l) ? (zset(f)) : zset((l)->next))):\
((zchk((n)->next) ? (0) : ((n)->next->prev=(n)->prev)),\
(zchk((n)->prev) ? (0) : ((n)->prev->next=(n)->next))))

#define QueuePush_NZ(f,l,n,next,zchk,zset) (zchk(f)?\
(((f)=(l)=(n)), zset((n)->next)):\
((l)->next=(n),(l)=(n),zset((n)->next)))
#define QueuePushFront_NZ(f,l,n,next,zchk,zset) (zchk(f) ? (((f) = (l) = (n)), zset((n)->next)) :\
((n)->next = (f)), ((f) = (n)))
#define QueuePop_NZ(f,l,next,zset) ((f)==(l)?\
(zset(f),zset(l)):\
((f)=(f)->next))

#define StackPush_N(f,n,next) ((n)->next=(f),(f)=(n))
#define StackPop_NZ(f,next,zchk) (zchk(f)?0:((f)=(f)->next))

#define DLLPushBack(f,l,n)       DLLPushBack_NPZ(f,l,n,next,prev,CheckNull,SetNull)
#define DLLPushFront(f,l,n)      DLLPushBack_NPZ(l,f,n,prev,next,CheckNull,SetNull)
#define DLLInsert(f,l,p,n)       DLLInsert_NPZ(f,l,p,n,next,prev,CheckNull,SetNull)
#define DLLRemove(f,l,n)         DLLRemove_NPZ(f,l,n,next,prev,CheckNull,SetNull)
#define QueuePush(f,l,n)         QueuePush_NZ(f,l,n,next,CheckNull,SetNull)
#define QueuePushFront(f,l,n)    QueuePushFront_NZ(f,l,n,next,CheckNull,SetNull)
#define QueuePop(f,l)            QueuePop_NZ(f,l,next,SetNull)
#define StackPush(f,n)           StackPush_N(f,n,next)
#define StackPop(f)              StackPop_NZ(f,next,CheckNull)

////////////////////////////
//- Helper Global Constants

#define max_u8  (u8)0xff
#define max_u16 (u16)0xffff
#define max_u32 (u32)0xffffffff
#define max_u64 (u64)0xffffffffffffffffllu

////////////////////////////
//- Helper Functions

static inline Vector2 world_ToScreen(Vector2 v);
static inline f32 rand_f32(f32 a, f32 b);

#endif // HELPERS_H
