// 2026-06-14

static Arena *
arena_alloc(u32 cap)
{
  Arena *result = (Arena*)malloc(cap);
  result->pos = ARENA_HEADER_SIZE;
  result->cap = cap;
  return result;
}

static void
arena_release(Arena *arena)
{
  free(arena);
}

static void *
arena_push(Arena *arena, u32 size, u32 align)
{
  u32 pos_aln = AlignPow2(arena->pos, align);
  u32 pos_new = pos_aln + size;
  void *result = 0;
  if(pos_new < arena->cap)
  {
    result = OffsetPtr(arena, pos_aln);
    arena->pos = pos_new;
  }
  else
  {
    // Log to the console.
  }
  return result;
}

static void
arena_pop_to(Arena *arena, u32 pos)
{
}

static void arena_pop(Arena *arena, u32 amt)
{
}

static void arena_clear(Arena* arena)
{
}
