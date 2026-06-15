// 2026-06-14

static force_inline void game_tick(void)
{
  game_updatePhysics();
  game_updateState();
  game_drawFrame();
}

static force_inline void game_mainLoop(void)
{
#if PLATFORM_WEB
  emscripten_set_main_loop(game_tick, 0, 1);
#else
  while(!WindowShouldClose()){game_tick();}
#endif
}
