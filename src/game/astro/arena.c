// 2026-06-14

static Arena *
arena_alloc(usize cap)
{
  Arena *result = (Arena*)malloc(cap);
  result->pos = ARENA_HEADER_SIZE;
  return result;
}

static void
arena_release(Arena *arena)
{
  free(arena);
}

static void *
arena_push(Arena *arena, usize size, u32 align)
{
  usize pos_aln = AlignPow2(arena->pos, align);
  usize pos_new = pos_aln + size;
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
arena_pop_to(Arena *arena, usize pos)
{
}

static void arena_pop(Arena *arena, usize amt)
{
}

static void arena_clear(Arena* arena)
{
}
