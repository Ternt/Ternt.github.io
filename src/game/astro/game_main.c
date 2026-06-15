// 2026-06-13
//
// TODO list (features, improvements, and bug fixes)
// -------------------------------------------------
// [ ] Resize window when resizing occurs in the 
//     browser. Currently window sizing is set 
//     only on startup. 
// [ ] Better memory allocator for model geometry
//     data.
// [ ] Revisit entity creation and destruction
//     logic. 


////////////////////////////
//- Third Party Includes

#include "raylib/raylib.h"
#include "raylib/raymath.h"
#include "raylib/rlgl.h"

////////////////////////////
//- Game Includes

#include "helpers.h"
#include "arena.h"
#include "physics.h"
#include "entity.h"
#include "game.h"

#include "helpers.c"
#include "arena.c"
#include "physics.c"
#include "entity.c"
#include "game.c"

////////////////////////////
//- Game Constants

#define PIXELS_PER_METER 5.0f

////////////////////////////
//- Game Globals

Arena *g_arena = 0;              // general memory pool
Entity player = NULL_ENTT;
Camera2D camera = {0};

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
  rlDisableBackfaceCulling();
  BeginDrawing();
  ClearBackground(BLACK);
    BeginMode2D(camera); // Draw Entities
      for(u32 i = 0; i < g_ecs_state->data_count; i += 1)
      {
        Entity entt = g_ecs_state->entities[i];
        Geometry *geo = &(g_ecs_state->geometries[i]);
        switch(geo->type)
        {
          case ShapeType_Triangle: {
            Transform *tn  = entt_getTransform(entt);
            Vector3 pos    = tn->translation;
            Quaternion rot = tn->rotation;
            Vector3 scale  = tn->scale;
            ShapeTriangle tri = geo->shape.triangle;
            rlPushMatrix();
              rlTranslatef(pos.x, pos.y, pos.z);
              rlScalef(scale.x, scale.y, 1.0f);
              DrawTriangle(world_toScreen(tri.v1), 
                           world_toScreen(tri.v2), 
                           world_toScreen(tri.v3), 
                           tri.color);
            rlPopMatrix();
          } break;

          case ShapeType_Rectangle: {
            Transform *tn  = entt_getTransform(entt);
            Vector3 pos    = tn->translation;
            Quaternion rot = tn->rotation;
            Vector3 scale  = tn->scale;
            ShapeRectangle rect = geo->shape.rectangle;
            Vector2 rect_pos  = (Vector2){-(rect.w/2.0), -(rect.h/2.0)};
            Vector2 rect_size = (Vector2){rect.w, rect.h};
            rlPushMatrix();
              rlTranslatef(pos.x, pos.y, pos.z);
              rlScalef(scale.x, scale.y, 1.0f);
              DrawRectangleV(world_toScreen(rect_pos), 
                             world_toScreen(rect_size), 
                             rect.color);
            rlPopMatrix();
          } break;

          case ShapeType_Circle: {
            Transform *tn  = entt_getTransform(entt);
            Vector3 pos    = tn->translation;
            Quaternion rot = tn->rotation;
            Vector3 scale  = tn->scale;
            ShapeCircle circle = geo->shape.circle;
            Vector2 circle_pos = (Vector2){pos.x, pos.y};
            rlPushMatrix();
              rlTranslatef(pos.x, pos.y, pos.z);
              rlScalef(scale.x, scale.y, 1.0f);
              DrawCircleV(world_toScreen(circle_pos), 
                          circle.radius, 
                          circle.color);
            rlPopMatrix();
          } break;
        }
      }
    EndMode2D();

    BeginUI(); // Draw UI
      DrawFPS(0, 0);
    EndUI();
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

    // spaceship player entity
    player = entt_create();
    entt_setTransform(player);

    // initialize camera
    w = GetScreenWidth();
    h = GetScreenHeight();
    float zoom = Min(w, h);
    Vector3 pos = entt_getPosition(player);
    camera.target   = (Vector2){ pos.x, pos.y };
    camera.offset   = (Vector2){ w/2.f, h/2.f };
    camera.rotation = 0.0f;
    camera.zoom     = 0.5 * zoom;

    // main game loop
    game_mainLoop();
  }
  ecs_quit();
  arena_release(g_arena);
}
