// 2026-06-14

static void entt_bufferInit(Arena *arena, u32 cap)
{
  g_entities.data = push_enttArray(arena, cap);
  g_entities.cap = cap;
  g_entities.count = 0;

  for(u32 i = 0; i < cap; i += 1)
  {
    g_entities.data[i] = NULL_ENTT;
  }
}

static Entity entt_create(void)
{
  b32 is_full = (g_entities.count >= g_entities.cap);
  Entity result = (is_full*NULL_ENTT) + ((!is_full)*g_entities.count);
  g_entities.count += (is_full*1);
  return result;
}

static void entt_destroy(Entity id)
{
  // FIX: Entity destruction and allocation
  // isn't as simple as I thought. Currently,
  // entity creation is done by g_entities.count,
  // this does not account for the fact that 
  // entity destruction is not linear. An entity
  // with any id can be destroyed at any given
  // time. This means we need some extra book-
  // keeping information. Perhaps we can split
  // the EntityBuffer into chunks. Where each
  // chunk has two 32-bit bitmask. One to
  // determine the chunk id, and the other to
  // determine entity location in that chunk.
  //
  // Or we can use a freelist. 
  g_entities.data[id] = NULL_ENTT;
  g_entities.count -= 1;
}

static void entt_set_component(Entity id, ComponentType type, void *in)
{
  if(id = NULL_ENTT){return;}

  switch(type)
  {
    case ComponentType_Transform: 
    {
      Transform *transform = &(g_transforms[id]);
      Transform data = *(Transform*)in;
      transform->translation = data.translation;
      transform->rotation = data.rotation;
      transform->scale = data.scale;
    } break;
  }
}
