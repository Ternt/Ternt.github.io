// 2026-06-17
//
// TODO list (features, improvements, and bug fixes)
// -------------------------------------------------
// [ ] Resize window when resizing occurs in the 
//     browser. Currently window sizing is set 
//     only on startup. 
// [ ] 

////////////////////////////
//- Third Party Includes

#if !defined(PLATFORM_WEB)
#include "raylib/external/glad.h"
#else
#include <GLES3/gl3.h>
#endif
#include "raylib/raylib.h"
#include "raylib/raymath.h"
#include "raylib/rlgl.h"

////////////////////////////
//- Game Configurations

// profiling macros
#if !defined(PLATFORM_WEB)
#define PROFILE_TRACY 1
#endif

// opengl version
#if defined(PLATFORM_WEB)
# ifndef GRAPHICS_API_OPENGL_ES3
# define GRAPHICS_API_OPENGL_ES3 1
# endif
#else
# ifndef GRAPHICS_API_OPENGL_33
# define GRAPHICS_API_OPENGL_33 1
# endif
#endif

////////////////////////////
//- Game Includes

#include "helpers.h"
#include "profiling.h"
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

GLuint      g_hexBuffer = 0;
NGon        g_hexShape = zero_struct;
Transform*  g_hexTransforms = null;
u32         g_hexInstCount = 1024;
f32         g_worldBoundX = 1000.0f;
f32         g_worldBoundY = 1000.0f;
R_PassArray g_passes = zero_struct;

////////////////////////////
//- Game Loop Pipeline

static inline void 
Game_UpdatePhysics(void)
{
  ProfBegin("Game_UpdatePhysics");
  ProfEnd();
}

static inline void 
Game_UpdateState(void)
{
  ProfBegin("Game_UpdateState");
  Game_HandleZoomAndDrag();
  ProfEnd();
}

static inline void 
Game_DrawWorld(void)
{
  R_Pass *pass = &g_passes.v[R_PassType_Geo2D];
  {
    // Initialize Geo2D Params
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    pass->paramsGeo2D.view = GetCameraMatrix2D(GAME.camera2D);
    pass->paramsGeo2D.proj = MatrixOrtho(0, w, h, 0, 0.0f, 10.0f);
  }

  ProfBegin("R_DrawAll");
  R_DrawAll(&g_passes);
  ProfEnd();
}

static void
Game_DrawUI(void)
{
}

static void
Game_DrawDebug(void)
{
  ProfBegin("Debug UI");

  // Screen-space debug
  Color fpsColor = LIME;
  u32   fps = GetFPS();
  if (InRange(fps, 15, 30)) fpsColor = ORANGE;
  if (InRange(fps,  0, 14)) fpsColor = RED;
  DrawTextEx(GAME.defaultFont, TextFormat("%2i FPS", fps), (Vector2){0, 0}, 20, 1, fpsColor);
  DrawTextEx(GAME.defaultFont, TextFormat("zoom %.3f\n", GAME.cameraZoom), (Vector2){0, 20}, 20, 1, WHITE);
  DrawTextEx(GAME.defaultFont, TextFormat("count %d\n", g_hexInstCount), (Vector2){0, 40}, 20, 1, WHITE);

  ProfEnd();
}

////////////////////////////
//- Game Entry Point (Game-specific Initializations)

static void Game_EntryPoint(int argc, char *argv[])
{
  g_hexShape = Geo_GenerateNGonConvex(GAME.arena, 6);

  g_hexTransforms = PushArray(GAME.arena, Transform, g_hexInstCount);
  for(u32 i = 0; i < g_hexInstCount; i += 1)
  {
    f32 randScale = rand_f32(1.0f, 2.5f);
    g_hexTransforms[i].scale = (Vector3){randScale, randScale, 1.0f};

    f32 randPosX = rand_f32(-0.5*g_worldBoundX, 0.5*g_worldBoundX);
    f32 randPosY = rand_f32(-0.5*g_worldBoundY, 0.5*g_worldBoundY);
    g_hexTransforms[i].translation = (Vector3){randPosX, randPosY, 0.0f};
  }

  // build rendering pipeline
  g_passes = R_MakePassArray();

  // geo2d pass
  R_Pass *pass = R_PushPass(&g_passes, R_PassType_Geo2D);
  {
    // Initialize Geo2D Params
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    pass->paramsGeo2D.view = GetCameraMatrix2D(GAME.camera2D);
    pass->paramsGeo2D.proj = MatrixOrtho(0, w, h, 0, 0.0f, 10.0f);

    // Initialize a 2D batch group for hexagonal instances
    {
      R_BatchGroup2DList *batchGroups = &pass->paramsGeo2D.batchGroups;

      // Allocate a batch group
      R_BatchGroup2DNode *node = PushArray(GAME.arena, R_BatchGroup2DNode, 1);
      QueuePush(batchGroups->first, batchGroups->last, node);
      batchGroups->node_count += 1;

      // Allocate static mesh vertices buffer.
      u32 size = g_hexShape.count * sizeof(Vector2);
      g_hexBuffer = R_AllocStaticBuffer(g_hexBuffer, size, g_hexShape.data);
      node->params.meshVertices = g_hexBuffer;
      node->params.vertCount = g_hexShape.count;

      ProfBegin("Push Instances");

      // Initialize group with N instances
      node->batches = R_MakeBatchList(sizeof(R_Hull2DInst));
      for(u32 i = 0; i < g_hexInstCount; i += 1)
      {
        R_Hull2DInst *hull_inst = (R_Hull2DInst*)R_PushBatchInst(GAME.arena, &node->batches, 32768);
        Transform tr = g_hexTransforms[i];
        Matrix model = MatrixIdentity();
        model = MatrixMultiply(model, MatrixScale(tr.scale.x, tr.scale.y, 1.0f));
        model = MatrixMultiply(model, MatrixTranslate(tr.translation.x, tr.translation.y, 0.0f));
        hull_inst->model = MatrixTranspose(model);
      }

      ProfEnd();
    }
  }
}

