// 2026-06-14

#define DEFAULT_FONT_PATH "astro/fonts/jetbrains-mono.ttf"

//
// helpers
//

static void Game_HandleZoomAndDrag(void)
{
  // Handle click dragging
  if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
  {
    Vector2 delta = GetMouseDelta();
    GAME.camera2D.target = Vector2Subtract(GAME.camera2D.target, Vector2Scale(delta, 1.0f/GAME.cameraZoom));
  }

  // Handle zooming
  Vector2 wheel = GetMouseWheelMoveV();
  GAME.cameraZoom += (GAME.cameraZoom * 0.1f * wheel.y);
  GAME.cameraZoom = Max(GAME.cameraZoom, 0.01f);
  GAME.camera2D.zoom = GAME.cameraZoom;
}

static void Game_DrawGrid(float w, float h, float cell_size, Color color)
{
  float half_w = w * 0.5f;
  float half_h = h * 0.5f;

  int cols = (u32)(w / cell_size);
  int rows = (u32)(h / cell_size);

  for(u32 i = 0; i <= cols; i += 1)
  {
    float x = -half_w + i * cell_size;
    DrawLineV((Vector2){x, -half_h}, (Vector2){x, half_h}, color);
  }

  for(u32 i = 0; i <= rows; i += 1)
  {
    float y = -half_h + i * cell_size;
    DrawLineV((Vector2){-half_w, y}, (Vector2){half_w, y}, color);
  }
}

static void Game_DrawOrigin(Color color)
{
  DrawLineV((Vector2){-1, 0}, (Vector2){1,0}, color);
  DrawLineV((Vector2){ 0,-1}, (Vector2){0,1}, color);
}


//
// main loop
//

static force_inline void Game_Tick(void)
{
  ProfScopeBegin();
  ProfBegin("Game_Tick");
  Game_UpdatePhysics();
  Game_UpdateState();

  BeginDrawing();
  ClearBackground(BLACK);
    Game_DrawWorld();
    Game_DrawUI();
    Game_DrawDebug();
  EndDrawing();

  ProfEnd(); 
  ProfScopeEnd();
}

static force_inline void Game_MainLoop(void)
{
#if PLATFORM_WEB
  emscripten_set_main_loop(Game_Tick, 0, 1);
#else
  while(!WindowShouldClose()){Game_Tick();}
#endif
}

int main(int argc, char *argv[])
{
  u32 w = atoi(argv[1]);
  u32 h = atoi(argv[2]);
  InitWindow(w,h,"Astro");

  // load font
  const char *font_path = TextFormat("%s/%s", GetWorkingDirectory(), DEFAULT_FONT_PATH);
  GAME.defaultFont = LoadFontEx(font_path, 32, 0, 0);

  // camera values
  w = GetScreenWidth();
  h = GetScreenHeight();
  GAME.cameraZoom = Min(w,h)/4.0f;

  // initialize camera
  GAME.camera2D.target   = (Vector2){0, 0};
  GAME.camera2D.offset   = (Vector2){w/2.f, h/2.f};
  GAME.camera2D.zoom     = GAME.cameraZoom;
  GAME.camera2D.rotation = 0.0f;

  // game entry point
  GAME.arena = ArenaAlloc(ARENA_DEFAULT_CAP);
  {
    R_Init();
    Game_EntryPoint(argc, argv);
    Game_MainLoop();
    R_Quit();
  }
  ArenaRelease(GAME.arena);

  // unload font
  UnloadFont(GAME.defaultFont);
}
