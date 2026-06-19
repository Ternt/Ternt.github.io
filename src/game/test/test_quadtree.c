// 2026-06-15

////////////////////////////
//- Third Party Includes

#include "raylib/raylib.h"
#include "raylib/raymath.h"
#include "raylib/rlgl.h"

////////////////////////////
//- Game Includes

#include "helpers.h"
#include "arena.h"
#include "quadtree.h"
#include "physics.h"
#include "entity.h"
#include "game.h"

#include "helpers.c"
#include "arena.c"
#include "quadtree.c"
#include "physics.c"
#include "entity.c"
#include "game.c"

////////////////////////////
//- Game Globals

Arena*   g_arena   = null;
Camera2D g_camera  = zero_struct;
float    g_zoom    = 1.0f;

////////////////////////////
//- Game Loop Pipeline

static void game_handleZoomAndDrag(void)
{
  // Handle click dragging
  if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
  {
    Vector2 delta = GetMouseDelta();
    g_camera.target = Vector2Subtract(g_camera.target, Vector2Scale(delta, 1.0f/g_zoom));
  }

  // Handle zooming
  Vector2 wheel = GetMouseWheelMoveV();
  g_zoom += (g_zoom * 0.1f * wheel.y);
  g_zoom = Max(g_zoom, 0.01f);
  g_camera.zoom = g_zoom;
}

static inline void 
game_updatePhysics(void)
{
}

static inline void 
game_updateState(void)
{
  game_handleZoomAndDrag();
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
            DrawCircleV(world_toScreen(circle.center), circle.radius, circle.color);
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
    DrawText(TextFormat("%f\n", g_zoom), 0, 20, 20, WHITE);
  EndUI();
  EndDrawing();
}

////////////////////////////
//- Game Entry Point

void game_entryPoint(int argc, char *argv[])
{
  ecs_init();
  g_arena = arena_alloc(ARENA_DEFAULT_CAP);
  {
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    g_zoom = Min(w, h);

    for(u32 i = 0; i < ECS_MAX_ENTITY; i += 1)
    {
      ecs_Entity entity_id = ecs_createEntity();

      Color      rand_color  = {rand_u8(100, 255), rand_u8(100, 255), rand_u8(100, 255), 255};
      float      rand_radius = rand_f32(1.0, 5.0);
      ecs_Circle rand_circle = {{0.0f, 0.0f}, rand_radius, rand_color};
      ecs_setGeometry(entity_id, .type=ecs_ShapeType_Circle, .shape=rand_circle);

      f32 rand_x = rand_f32(-1000.f, 1000.f);
      f32 rand_y = rand_f32(-1000.f, 1000.f);
      Vector3 rand_pos = {.x=rand_x, .y=rand_y};
      ecs_setTransform(entity_id, .p=rand_pos);
    }

    // initialize camera
    g_camera.target   = (Vector2){0, 0};
    g_camera.offset   = (Vector2){w/2.f, h/2.f};
    g_camera.zoom     = 0.25 * g_zoom;
    g_camera.rotation = 0.0f;

    // main game loop
    game_mainLoop();
  }
  arena_release(g_arena);
  ecs_quit();
}

