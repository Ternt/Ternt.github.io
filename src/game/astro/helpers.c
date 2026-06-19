// 2026-06-14

static inline Vector2 world_toScreen(Vector2 v)
{
  return (Vector2){ v.x, -v.y };
}

static inline f32 rand_f32(f32 a, f32 b)
{
  return a + (f32)rand() / (f32)(RAND_MAX) * (b - a);
}

static inline u8 rand_u8(u8 a, u8 b)
{
  return a + rand() % (b - a + 1);
}
