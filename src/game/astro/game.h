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
//- Application Structure Functions

// defined by the user
static inline void game_updatePhysics(void);
static inline void game_updateState(void);
static inline void game_drawFrame(void);

// internal, not-user defined
static force_inline void game_tick(void);
static force_inline void game_mainLoop(void);

// ui marker macros (does nothing, code decoration)
#define BeginUI() (void)(0)
#define EndUI() (void)(0)

#endif // GAME_H
