// 2026-06-14

static inline Vector2 world_toScreen(Vector2 v)
{
  return (Vector2){ v.x, -v.y };
}
