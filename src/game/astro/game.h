// 2026-06-14
//
// Abstracts the code difference between 
// browser and desktop platform.

#ifndef GAME_H
#define GAME_H

////////////////////////////
//- Emscripten Includes

#if PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

////////////////////////////
//- Game Mega Struct

typedef struct Game {
  Arena *arena;
  Font defaultFont;
  Camera2D camera2D;
  f32 cameraZoom;
} Game;

////////////////////////////
//- Game Globals

static Game GAME = zero_struct;

////////////////////////////
//- Game World Functions

// helpers
static void Game_HandleZoomAndDrag(void);
static void Game_DrawGrid(f32 w, f32 h, f32 cell_size, Color color);

////////////////////////////
//- Application Structure Functions

// defined by the user
static inline void Game_UpdatePhysics(void);
static inline void Game_UpdateState(void);
static inline void Game_DrawWorld(void);
static inline void Game_DrawUI(void);
static inline void Game_DrawDebug(void);
static void Game_EntryPoint(int argc, char *argv[]);

// main loop, internal, not-user defined
static force_inline void Game_Tick(void);
static force_inline void Game_MainLoop(void);

// ui marker macros (does nothing, code decoration)
#define BeginUI() (void)(0)
#define EndUI() (void)(0)

#endif // GAME_H
