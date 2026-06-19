// 2026-06-17

#ifndef GEOMETRY_H
#define GEOMETRY_H

////////////////////////////
//- Geometry Constants

#define GEO_MAX_NGON_SIDES 12
#define GEO_MAX_NGON_COUNT 1024
#define GEO_DEFAULT_INNER_RADIUS_PCT 0.5

////////////////////////////
//- Geometry Type Definitions

typedef struct NGon {
  Vector2 *data;
  u32 count;
} NGon;

////////////////////////////
//- Geometry Functions

static NGon Geo_GenerateNGonConvex(Arena *arena, u32 sides);
static NGon Geo_GenerateNGonConcave(Arena *arena, u32 sides, f32 inner_radius_pct);

#endif // GEOMETRY_H
