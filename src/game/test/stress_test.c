// 2026-06-14

////////////////////////////
//- Third Party Includes

#include "raylib/raylib.h"
#include "raylib/raymath.h"
#include "raylib/rlgl.h"

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

Arena*     g_arena  = NULL;
ecs_Entity g_player = NULL_ENTITY;
Camera2D   g_camera = ZERO_STRUCT;

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
  BeginMode2D(g_camera);
    for(u32 i = 0; i < g_ecs_state->data_count; i += 1)
    {
      ecs_Entity entt_id = g_ecs_state->entt_idx[i];
      ecs_Geometry *geo = ecs_getGeometry(entt_id);
      switch(geo->type)
      {
///////// Draw Circle /////////////////////////////////////////////////////////////////////////////////////////////////
        case ecs_ShapeType_Circle: {
          ecs_Transform *tn = ecs_getTransform(entt_id);
          Vector3 p = tn->p;
          Vector4 r = tn->r;
          Vector3 s = tn->s;

          rlPushMatrix();
            rlTranslatef(p.x, p.y, p.z);
            rlScalef(s.x, s.y, s.z);
            ecs_Circle circle = geo->shape.circle;
            DrawCircleV(world_toScreen(circle.center), 
                        circle.radius, 
                        circle.color);
          rlPopMatrix();
        } break;

///////// Draw Triangle ///////////////////////////////////////////////////////////////////////////////////////////////
        case ecs_ShapeType_Triangle: {
          ecs_Transform *tn = ecs_getTransform(entt_id);
          Vector3 p = tn->p;
          Vector4 r = tn->r;
          Vector3 s = tn->s;

          rlPushMatrix();
            rlTranslatef(p.x, p.y, p.z);
            rlScalef(s.x, s.y, s.z);
            ecs_Triangle tri = geo->shape.tri;
            DrawTriangle(world_toScreen(tri.v1), 
                         world_toScreen(tri.v2), 
                         world_toScreen(tri.v3), 
                         tri.color);
          rlPopMatrix();
        } break;

///////// Draw Rectangle //////////////////////////////////////////////////////////////////////////////////////////////
        case ecs_ShapeType_Rectangle: {
          ecs_Transform *tn = ecs_getTransform(entt_id);
          Vector3 p = tn->p;
          Vector4 r = tn->r;
          Vector3 s = tn->s;

          rlPushMatrix();
            rlTranslatef(p.x, p.y, p.z);
            rlScalef(s.x, s.y, s.z);
            ecs_Rectangle rect = geo->shape.rect;
            Vector2 rect_pos   = (Vector2){-(rect.w/2.0f), -(rect.h/2.0f)};
            Vector2 rect_size  = (Vector2){rect.w, rect.h};
            DrawRectangleV(rect_pos, rect_size, rect.color);
          rlPopMatrix();
        } break;
      }
    }
  EndMode2D();

///////// Draw UI /////////////////////////////////////////////////////////////////////////////////////////////////////
  BeginUI();
    DrawFPS(0, 0);
  EndUI();
  EndDrawing();
}

////////////////////////////
//- Game Entry Point

int main(int argc, char *argv[])
{
  ecs_init();
  g_arena = arena_alloc(ARENA_DEFAULT_CAP);
  {
    // initialize window
    int w = atoi(argv[1]);
    int h = atoi(argv[2]);
    InitWindow(w, h, "raylib wasm demo");

    // player entity
    ecs_Entity g_player = ecs_createEntity();
    ecs_setGeometry(g_player);
    ecs_setTransform(g_player);

    w = GetScreenWidth();
    h = GetScreenHeight();
    float zoom = Min(w, h);
    Vector3 pos = ecs_getTransform(g_player)->p;

    // initialize camera
    g_camera.target   = (Vector2){pos.x, pos.y};
    g_camera.offset   = (Vector2){w/2.f, h/2.f};
    g_camera.zoom     = 0.5f*zoom;
    g_camera.rotation = 0.0f;

    // main game loop
    game_mainLoop();
  }
  arena_release(g_arena);
  ecs_quit();
}

