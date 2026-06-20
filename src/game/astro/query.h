// 2026-06-15
//
// Contains code for a data-structure used 
// to accelerate querying of spatial data.

#ifndef QUERY_H
#define QUERY_H

/////////////////////////////
//- Spatial Query Type Definitions

typedef struct Qre_Node Qre_Node;
struct Qre_Node {
  Qre_Node *first;
  Qre_Node *last;
  Qre_Node *next;
};

typedef struct Qre_Entity {
  Vector2 pos;
  Vector2 bounds;
} Qre_Entity;

typedef struct Qre_Result {
  Qre_Entity *entities;
  u32 count;
} Qre_Result;

typedef struct Qre_Ctx {
  Vector2 bounds;
} Qre_Ctx;

/////////////////////////////
//- Spatial Query Functions

// main interface
static Qre_Ctx     Qre_MakeContext(Arena *arena, Vector2 bounds);
static Qre_Entity *Qre_InsertEntity(Qre_Ctx *ctx, Vector2 pos, Vector2 bounds);
static void        Qre_UpdateEntity(Qre_Ctx *ctx, Qre_Entity *entity);
static Qre_Result  Qre_FindNearby(Qre_Ctx *ctx, Vector2 pos, Vector2 bounds);
static void        Qre_RemoveEntity(Qre_Ctx *ctx, Qre_Entity *entity);

#endif // QUERY_H
