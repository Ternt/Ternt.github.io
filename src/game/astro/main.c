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
#include <emscripten/emscripten.h>

////////////////////////////
//- Game Includes

#include "helpers.h"
#include "arena.h"
#include "entity.h"
// #include "physics.h"

#include "helpers.c"
#include "arena.c"
#include "entity.c"
// #include "physics.c"

////////////////////////////
//- Game Globals

Arena *g_arena = 0;              // general memory pool
Arena *g_geo_pool = 0;           // geometry memory pool
usize  g_geo_pool_size = KB(16); // memory pool size
Entity entt_player = NULL_ENTT;  // player entity

////////////////////////////
//- Game Loop Pipeline

static void 
game_processEvents(void)
{
}

static void 
game_updateState(void)
{
}

static void 
game_drawFrame(void)
{
  BeginDrawing();
  ClearBackground(BLACK);
  EndDrawing();
}

static void 
game_mainLoop(void)
{
  game_processEvents();
  game_updateState();
  game_drawFrame();
}

////////////////////////////
//- Game Entry Point

int main(int argc, char *argv[])
{
  g_arena = arena_alloc(ARENA_DEFAULT_CAP);
  g_geo_pool = arena_alloc(g_geo_pool_size);
  {
    // initialize window
    int w = atoi(argv[1]);
    int h = atoi(argv[2]);
    InitWindow(w, h, "raylib wasm demo");

    entt_bufferInit(g_arena, ENTT_MAX);

    // spaceship player entity
    entt_player = entt_create();

    // set main game loop
    emscripten_set_main_loop(game_mainLoop, 0, 1);
  }
  arena_release(g_arena);
  arena_release(g_geo_pool);
}
