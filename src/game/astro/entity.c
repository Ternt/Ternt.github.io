// 2026-06-14

//
// initialization
//

static void ecs_init(void)
{
  Arena *arena = arena_alloc(ARENA_DEFAULT_CAP);
  g_ecs_state = push_array(arena, ecs_State, 1);

  // Initialize Entities Buffer.
  g_ecs_state->free_count = ECS_MAX_ENTITY;
  g_ecs_state->data_count = 0;

  for(u16 i = 0; i < ECS_MAX_ENTITY; i += 1)
  {
    g_ecs_state->entt_idx[i] = NULL_ENTITY;
  }

  // initialize in reverse so that the
  // entity at idx position ECS_MAX_ENTITY 
  // is the 0th index.
  for(u16 i = ECS_MAX_ENTITY; i > 0; i -= 1)
  {
    u32 idx = ECS_MAX_ENTITY - i;
    g_ecs_state->free_entt_idx[i - 1] = idx;
  }
}

static void ecs_quit(void)
{
  arena_release(g_ecs_state->arena);
}


//
// entity creation/destruction
//

static ecs_Entity ecs_createEntity(void)
{
  ecs_Entity result = NULL_ENTITY;
  if(g_ecs_state->free_count > 0)
  {
    g_ecs_state->free_count -= 1;
    u32 free_idx = g_ecs_state->free_count;
    result = g_ecs_state->free_entt_idx[free_idx];
    g_ecs_state->free_entt_idx[free_idx] = max_u16;

    u32 data_idx = g_ecs_state->data_count;
    g_ecs_state->entt_idx[data_idx] = result;
    g_ecs_state->data_count += 1;
  }
  return result;
}

static void ecs_destroyEntity(ecs_Entity id)
{
  g_ecs_state->entt_idx[id] = NULL_ENTITY;
  u32 idx = g_ecs_state->free_count;
  g_ecs_state->free_entt_idx[idx] = id;
  g_ecs_state->free_count += 1;
  g_ecs_state->data_count -= 1;
}


//
// component helper functions
//

// NOTE: for now, it is a linear mapping.
static u32 ecs_mapEntityToComponent(ecs_Entity id, ecs_ComponentType type)
{
  u32 result = 0;
  switch(type)
  {
    case ecs_ComponentType_Transform: {
      result = id;
    } break;
  }
  return result;
}


//
// component setter functions
//

static void ecs_setTransform_ex(ecs_Entity id, ecs_Transform *in)
{
  u32 component_id = ecs_mapEntityToComponent(id, ecs_ComponentType_Transform);
  ecs_Transform *t = &g_ecs_state->transforms[component_id];
  MemoryCopy(t, in, sizeof(ecs_Transform));
}

static void ecs_setGeometry_ex(ecs_Entity id, ecs_Geometry *in)
{
  u32 component_id = ecs_mapEntityToComponent(id, ecs_ComponentType_Geometry);
  ecs_Geometry *t = &g_ecs_state->geometries[component_id];
  MemoryCopy(t, in, sizeof(ecs_Geometry));
}


//
// component getter functions
//

static void *ecs_getComponent(ecs_Entity id, ecs_ComponentType type)
{
  void *result = 0;
  u32 component_id = ecs_mapEntityToComponent(id, ecs_ComponentType_Geometry);
  switch(type)
  {
    case ecs_ComponentType_Transform: {
      result = &(g_ecs_state->transforms[component_id]);
    } break;

    case ecs_ComponentType_Geometry: {
      result = &(g_ecs_state->geometries[component_id]);
    } break;
  }
  return result;
}

