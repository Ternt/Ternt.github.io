// 2026-06-17

static NGon Geo_GenerateNGonConvex(Arena *arena, u32 sides)
{
  Vector2 *data = PushArray(arena, Vector2, sides);
  f32 angle_inc = 360.0f / (f32)sides;
  for(u32 i = 0; i < sides; i += 1)
  {
    f32 angle = angle_inc * i;
    f32 rad = angle * DEG2RAD;
    f32 x = sinf(rad);
    f32 y = cosf(rad);
    data[i] = (Vector2){x,y};
  }

  NGon result = zero_struct;
  result.data = data;
  result.count = sides;
  return result;
}

static NGon Geo_GenerateNGonConcave(Arena *arena, u32 sides, f32 inner_radius_pct)
{
  // Generate points on unit circle.
  Vector2 temp[GEO_MAX_NGON_SIDES] = zero_struct;
  f32 angle_inc = 360.0f / (f32)sides;
  for(u32 i = 0; i < sides; i += 1)
  {
    f32 angle = angle_inc * i;
    f32 rad = angle * DEG2RAD;
    f32 x = sinf(rad);
    f32 y = cosf(rad);
    temp[i] = (Vector2){x,y};
  }

  // Deform some points towards inner radius 
  // percentage to make the N-gon concave.
  Vector2 *data = PushArray(arena, Vector2, sides * 3);
  for(u32 i = 0; i < sides - 1; i += 1)
  {
  }

  NGon result = zero_struct;
  result.data = data;
  result.count = 3*sides;
  return result;
}

