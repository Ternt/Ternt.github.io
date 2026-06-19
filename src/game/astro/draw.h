// 2026-06-19

#ifndef DRAW_H
#define DRAW_H

////////////////////////////
//- Draw Type Definitions



////////////////////////////
//- Draw Functions

static void DR_DrawNGonConvex(Vector2 pos, f32 scale, u32 sides, b32 random);
static void DR_DrawNGonConcave(Vector2 pos, f32 scale, u32 sides, b32 random);

static void DR_DrawBegin(Arena *arena);
static void DR_DrawEnd(void);

#endif // DRAW_H
