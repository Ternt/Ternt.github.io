// 2026-06-14

////////////////////////////
//- Third Party Includes

#include "raylib/raylib.h"
#include "raylib/raymath.h"

////////////////////////////
//- Game Includes

#include "helpers.h"
#include "arena.h"
#include "entity.h"
#include "game.h"

#include "helpers.c"
#include "arena.c"
#include "entity.c"
#include "game.c"

////////////////////////////
//- Game Globals

Arena *g_arena = 0; // general memory pool

////////////////////////////
//- Game Loop Pipeline

static inline void 
game_updatePhysics(void)
{
}

static inline void 
game_updateState(void)
{
}

static inline void 
game_drawFrame(void)
{
  BeginDrawing();
  ClearBackground(BLACK);
  // BeginMode2D(camera);
  // {
  //   for(u32 i = 0; i < g_ecs_state->data_count; i += 1)
  //   {
  //     Entity entt = g_ecs_state->entities[i];
  //     Geometry *geo = &(g_ecs_state->geometries[i]);
  //     switch(geo->type)
  //     {
  //       case ShapeType_Triangle: {
  //         Vector3  pos = entt_getPosition(entt);
  //         Triangle tri = geo->shape.triangle;
  //         tri.v1 = Vector2Add(tri.v1, (Vector2){pos.x,pos.y});
  //         tri.v2 = Vector2Add(tri.v2, (Vector2){pos.x,pos.y});
  //         tri.v3 = Vector2Add(tri.v3, (Vector2){pos.x,pos.y});
  //         DrawTriangle(tri.v1, tri.v2, tri.v3, tri.color);
  //       } break;
  //     }
  //   }
  // }
  // EndMode2D();
  EndDrawing();
}

////////////////////////////
//- Game Entry Point

int main(int argc, char *argv[])
{
  g_arena = arena_alloc(ARENA_DEFAULT_CAP);
  ecs_init();
  {
    // initialize window
    int w = atoi(argv[1]);
    int h = atoi(argv[2]);
    InitWindow(w, h, "raylib wasm demo");

    for(u32 i = 0; i < ENTT_MAX; i += 1)
    {
      entt_setTransform(player, &TRANSFORM_DEFAULT);
      entt_setGeometry(player, &GEOMETRY_TRIANGLE);
    }

    // main game loop
    game_mainLoop();
  }
  ecs_quit();
  arena_release(g_arena);
}
