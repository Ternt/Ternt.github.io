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

GLuint g_hexBuffer = 0;
NGon   g_hexShape = zero_struct;

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
  Temp temp = TempBegin(GAME.arena);
  {
    // build rendering pipeline
    R_PassArray passes = R_MakePassArray();

    // geo2d pass
    R_Pass *pass = R_PushPass(&passes, R_PassType_Geo2D);
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
        R_BatchGroup2DNode *node = PushArray(temp.arena, R_BatchGroup2DNode, 1);
        QueuePush(batchGroups->first, batchGroups->last, node);
        batchGroups->node_count += 1;

        // Allocate static mesh vertices buffer.
        u32 size = g_hexShape.count * sizeof(Vector2);
        g_hexBuffer = R_AllocStaticBuffer(g_hexBuffer, size, g_hexShape.data);
        node->params.meshVertices = g_hexBuffer;
        node->params.vertCount = g_hexShape.count;

        // Initialize group with N instances
        node->batches = R_MakeBatchList(sizeof(R_Hull2DInst));
        for(u32 i = 0; i < 1; i += 1)
        {
          R_Hull2DInst *hull_inst = (R_Hull2DInst*)R_PushBatchInst(temp.arena, &node->batches, 128);
          Matrix model = MatrixIdentity();
        }
      }
    }

    R_DrawAll(&passes);
  }
  TempEnd(temp);
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
  g_hexShape = Geo_GenerateNGonConvex(GAME.arena, 6);
}

