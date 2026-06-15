// 2026-06-14

#ifndef ENTITY_H
#define ENTITY_H

////////////////////////////
//- ECS Constants

#define ECS_MAX_ENTITY 1024
#define NULL_ENTITY max_u16

////////////////////////////
//- ECS Component Type Definitions

typedef enum ecs_ComponentType {
  ecs_ComponentType_NULL      = 0,
  ecs_ComponentType_Transform = 1,
  ecs_ComponentType_Geometry  = 2,
  ecs_ComponentType_Sprite    = 3,
  ecs_ComponentType_Physics   = 4,
  ecs_ComponentType_COUNT,
} ecs_ComponentType;

typedef struct ecs_Transform {
  Vector3 p;
  Vector4 r; // Quaternion
  Vector3 s;
} ecs_Transform;

typedef enum ecs_ShapeType {
  ecs_ShapeType_NULL      = 0,
  ecs_ShapeType_Circle    = 1,
  ecs_ShapeType_Triangle  = 2,
  ecs_ShapeType_Rectangle = 3,
  ecs_ShapeType_Custom    = 4,
  ecs_ShapeType_COUNT,
} ecs_ShapeType;

typedef struct ecs_Circle {
  Vector2 center;
  float radius;
  Color color;
} ecs_Circle;

typedef struct ecs_Triangle {
  Vector2 v1;
  Vector2 v2;
  Vector2 v3;
  Color color;
} ecs_Triangle;

typedef struct ecs_Rectangle {
  Vector2 pos;
  Vector2 size;
  Color color;
} ecs_Rectangle;

typedef Model ecs_Model;

typedef union ecs_Shape {
  ecs_Circle circle;
  ecs_Triangle tri;
  ecs_Rectangle rect;
  ecs_Model custom;
} ecs_Shape;

typedef struct ecs_Geometry {
  ecs_ShapeType type;
  ecs_Shape shape;
} ecs_Geometry;

////////////////////////////
//- ECS Entity Type Definitions

// Abstract entity
typedef u16 ecs_Entity;

////////////////////////////
//- ECS System State

typedef struct ecs_State {
  Arena *arena;
  ecs_Geometry  geometries[ECS_MAX_ENTITY]; // geometry component buffer
  ecs_Transform transforms[ECS_MAX_ENTITY]; // transform component buffer

  ecs_Entity entt_idx[ECS_MAX_ENTITY];      // entities buffer
  ecs_Entity free_entt_idx[ECS_MAX_ENTITY]; // free entities list
  u16 data_count, free_count; 
} ecs_State;

////////////////////////////
//- Entity Globals

ecs_State *g_ecs_state = NULL;

////////////////////////////
//- Component Constants

#define ECS_DEFAULT_ROTATION (Vector4){ 0.0f, 0.0f, 0.0f, 1.0f }
#define ECS_DEFAULT_POSITION (Vector3){ 0.0f, 0.0f, 0.0f }
#define ECS_DEFAULT_SCALE    (Vector3){ 1.0f, 1.0f, 1.0f }
#define ECS_SHAPE_CIRCLE     .type=ecs_ShapeType_Circle,                                  \
                             .shape.circle=(ecs_Circle){(Vector2){0.0f, 0.0f}, 1.0f, RED}
#define ECS_SHAPE_RECTANGLE  .type=ecs_ShapeType_Rectangle,                               \
                             .shape.rect=(ecs_Rectangle){(Vector2){  0.0f,  0.0f },       \
                                                         (Vector2){  1.0f,  1.0f }, RED}
#define ECS_SHAPE_TRIANGLE   .type=ecs_ShapeType_Triangle,                                \
                             .shape.tri=(ecs_Triangle){(Vector2){  0.0f,  1.0f },         \
                                                       (Vector2){ -1.0f, -1.0f },         \
                                                       (Vector2){  1.0f, -1.0f }, RED}
#define ECS_DEFAULT_SHAPE ECS_SHAPE_TRIANGLE

////////////////////////////
//- Entity Functions

// initialization
static void ecs_init(void);
static void ecs_quit(void);

// entity creation/destruction
static ecs_Entity ecs_createEntity(void);
static void entt_destroyEntity(ecs_Entity id);

// component helper functions
static u32 ecs_mapEntityToComponent(ecs_Entity id, ecs_ComponentType type);

// component setter functions
static void ecs_setTransform_ex(ecs_Entity id, ecs_Transform *in);
static void ecs_setGeometry_ex(ecs_Entity id, ecs_Geometry *in);
#define ecs_setTransform(id, ...) ecs_setTransform_ex(id, &(ecs_Transform){ .p = ECS_DEFAULT_POSITION, .r = ECS_DEFAULT_ROTATION, .s = ECS_DEFAULT_SCALE, __VA_ARGS__ })
#define ecs_setGeometry(id, ...) ecs_setGeometry_ex(id, &(ecs_Geometry){ ECS_DEFAULT_SHAPE, __VA_ARGS__ })

// component getter functions
static void *ecs_getComponent(ecs_Entity id, ecs_ComponentType type);
#define ecs_getTransform(id) ((ecs_Transform*)ecs_getComponent(id, ecs_ComponentType_Transform))
#define ecs_getGeometry(id) ((ecs_Geometry*)ecs_getComponent(id, ecs_ComponentType_Geometry))

#endif // ENTITY_H
