// 2026-06-14

#ifndef ENTITY_H
#define ENTITY_H

////////////////////////////
//- Entity Type Definitions

// Abstract entity
typedef u32 Entity;

// Entity buffer
typedef struct EntityBuffer {
  Entity *data;
  u32 cap;
  u32 count;
} EntityBuffer;

////////////////////////////
//- Component Type Definitions

typedef enum ComponentType {
  ComponentType_Invalid   = 0,
  ComponentType_Transform = 1,
  ComponentType_Geometry  = 2,
  ComponentType_RigidBodyPhysics = 3,
  ComponentType_COUNT,
} ComponentType;

typedef struct Geometry {
  f64 *data;
  u32 count;
} Geometry;

////////////////////////////
//- Entity Constants

#define ENTT_MAX 1024
#define NULL_ENTT max_u32

////////////////////////////
//- Entity Globals

// entities array
EntityBuffer g_entities = {0};

// different components array
Transform g_transforms[ENTT_MAX] = {0};
Geometry g_geometrys[ENTT_MAX] = {0};

////////////////////////////
//- Entity Functions

// core entity functions
static void   entt_bufferInit(Arena *arena, u32 cap);
static Entity entt_create(void);
static void   entt_destroy(Entity id);
static void   entt_set_component(Entity id, ComponentType type, void *in);

// entity helper macros
#define push_enttArray(a,c) push_array(a,Entity,c)
#define push_entt(a) push_struct(a,Entity)

#endif // ENTITY_H
