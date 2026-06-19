// 2026-06-14

static Arena *
ArenaAlloc(u32 cap)
{
  Arena *result = (Arena*)malloc(cap);
  result->pos = ARENA_HEADER_SIZE;
  result->cap = cap;
  return result;
}

static void
ArenaRelease(Arena *arena)
{
  free(arena);
}

static void *
ArenaPush(Arena *arena, u32 size, u32 align)
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
ArenaPopTo(Arena *arena, u32 pos)
{
  pos = ClampTop(pos, arena->cap);
}

static void ArenaPop(Arena *arena, u32 amt)
{
  u32 old_amt = arena->pos;
  u32 new_amt = old_amt;
  if(amt < old_amt)
  {
    new_amt -= amt;
  }
  ArenaPopTo(arena, new_amt);
}

static void ArenaClear(Arena* arena)
{
  ArenaPopTo(arena, 0);
}

static Temp TempBegin(Arena *arena)
{
  Temp temp = {arena, arena->pos};
  return temp;
}

static void TempEnd(Temp temp)
{
  Arena *arena = temp.arena;
  ArenaPopTo(arena, arena->pos);
}

