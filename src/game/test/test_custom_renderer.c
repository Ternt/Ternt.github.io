// 2026-06-17

////////////////////////////
//- Third Party Includes

#ifndef PLATFORM_WEB
#include "raylib/external/glad.h"
#else
#include <GLES3/gl3.h>
#endif
#include "raylib/raylib.h"
#include "raylib/raymath.h"
#include "raylib/rlgl.h"

////////////////////////////
//- Game Includes

#include "helpers.h"
#include "arena.h"
#include "render.h"
#include "geometry.h"
#include "game.h"

#include "helpers.c"
#include "arena.c"
#include "render.c"
#include "geometry.c"
#include "game.c"

////////////////////////////
//- Game Globals

Arena*      g_arena  = null;
R_PassArray g_passes = zero_struct;

////////////////////////////
//- Game Loop Pipeline

static inline void 
Game_UpdatePhysics(void)
{
}

static inline void 
Game_UpdateState(void)
{
  Game_HandleZoomAndDrag();
}

static inline void 
Game_DrawWorld(void)
{
  R_DrawAll(&g_passes);
}

static void
Game_DrawUI(void)
{
}

static void
Game_DrawDebug(void)
{
  // Screen-space debug
  Color fpsColor = LIME;
  u32   fps = GetFPS();
  if (InRange(fps, 15, 30)) fpsColor = ORANGE;
  if (InRange(fps,  0, 14)) fpsColor = RED;
  DrawTextEx(GAME.defaultFont, TextFormat("%2i FPS", fps), (Vector2){0, 0}, 20, 1, fpsColor);
  DrawTextEx(GAME.defaultFont, TextFormat("zoom %.3f\n", GAME.cameraZoom), (Vector2){0, 20}, 20, 1, WHITE);
}

////////////////////////////
//- Game Entry Point (Game-specific Initializations)

static void Game_EntryPoint(int argc, char *argv[])
{
  g_arena = ArenaAlloc(ARENA_DEFAULT_CAP);
  {
    g_passes = R_MakePassArray();
    R_Pass *pass = R_PushPass(&g_passes, R_PassType_Geo2D);
    R_BatchList list = R_MakeBatchList(64);
    void *inst = R_PushBatchInst(g_arena, &list, 1024);
  }
  ArenaRelease(g_arena);
}

