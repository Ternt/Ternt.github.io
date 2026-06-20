// 2026-06-17

////////////////////////////
//- Default Shader Info Tables

#include "./shaders.c"

const R_Attribs r_geo_shader_input_attribs[] = 
{
  { 0, "in_pos",   GL_FLOAT, 2 },
  { 1, "in_model", GL_FLOAT, 4 },
};

const R_Attribs r_geo_shader_output_attribs[] = 
{
  { 0, "in_pos", GL_FLOAT, 2 },
};

const R_AttribsArray r_shader_input_attribs[R_PassType_COUNT] =
{
  { r_geo_shader_input_attribs, ArrayCount(r_geo_shader_input_attribs) },
};

const R_AttribsArray r_shader_output_attribs[R_PassType_COUNT] =
{
  { r_geo_shader_output_attribs, ArrayCount(r_geo_shader_output_attribs) },
};


////////////////////////////
//- Render Functions

static GLenum R_GL_BufferToVBOType(R_BufferType type)
{
  GLenum result = 0;
  switch(type)
  {
    case R_BufferType_Static:
    {
      result = GL_STATIC_DRAW;
    } break;
    case R_BufferType_Dynamic:
    {
      result = GL_DYNAMIC_DRAW;
    } break;
    case R_BufferType_Stream:
    {
      result = GL_STREAM_DRAW;
    } break;
  }
  return result;
}

static GLuint
R_AllocBuffer(R_BufferType type, u32 size, void *data)
{
  GLuint buffer = 0;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, size, data, R_GL_BufferToVBOType(type));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return buffer;
}

static void
R_ReleaseBuffer(GLuint buffer)
{
  glDeleteBuffers(1, &buffer);
}

static GLuint R_AllocStaticBuffer(GLuint handle, u32 size, void *data)
{
  GLuint result = handle;
  if(result == 0)
  {
    result = R_AllocBuffer(R_BufferType_Static, size, data);
  }
  return result;
}

static R_PassArray R_MakePassArray(void)
{
  R_PassArray passes = zero_struct;
  for(u32 i = 0; i < R_PassType_COUNT; i += 1)
  {
    R_Pass *pass = &passes.v[i];
    pass->type = R_PassType_NULL;
  }
  return passes;
}

static R_Pass *R_PushPass(R_PassArray *passes, R_PassType type)
{
  R_Pass *pass = &passes->v[passes->count];
  if(pass->type == R_PassType_NULL)
  {
    pass->type = type;
    passes->count += 1;
  }
  return pass;
}

static R_BatchList R_MakeBatchList(u32 inst_size)
{
  R_BatchList result = zero_struct;
  result.inst_size = inst_size;
  return result;
}

static void *R_PushBatchInst(Arena *arena, R_BatchList *list, u32 batch_inst_cap)
{
  void *inst = null;
  {
    R_BatchNode *n = list->last;

    // push new batch onto the batch list if 
    // there is no space or if the list is empty.
    if(n == null || n->v.batch_size+list->inst_size > n->v.batch_cap)
    {
      n = PushArray(arena, R_BatchNode, 1);
      n->v.batch_cap = batch_inst_cap*list->inst_size;
      n->v.bytes = PushArray(arena, u8, n->v.batch_cap); 
      QueuePush(list->first, list->last, n);
      list->batch_count += 1;
    }
    
    // push new instance into the batch.
    inst = n->v.bytes + n->v.batch_size;
    n->v.batch_size += list->inst_size;
    list->batch_total_size += list->inst_size;
  }
  return inst;
}


static void R_Init(void)
{
  Arena *arena = ArenaAlloc(ARENA_DEFAULT_CAP);
  RENDER = PushArray(arena, R_RenderState, 1);
  RENDER->arena = arena;

  // initialize default shaders
  for(u32 k = 0; k < R_PassType_COUNT; k += 1)
  {
    struct {GLenum type; const char *src_data; u32 src_size; GLuint out; char *err;} stages[] =
    {
      {GL_VERTEX_SHADER,   r_vs_shader_src[k].data, r_vs_shader_src[k].size},
      {GL_FRAGMENT_SHADER, r_ps_shader_src[k].data, r_ps_shader_src[k].size},
    };

    // compile shaders stages.
    for(u32 i = 0; i < ArrayCount(stages); i += 1)
    {
      GLint src_size = 0;
      stages[i].out = glCreateShader(stages[i].type);
      glShaderSource(stages[i].out, 1, (char**)&stages[i].src_data, &stages[i].src_size);
      glCompileShader(stages[i].out);
      GLint info_log_length = 0;
      GLint status = 0;
      glGetShaderiv(stages[i].out, GL_COMPILE_STATUS, &status);
      glGetShaderiv(stages[i].out, GL_INFO_LOG_LENGTH, &info_log_length);
      if(info_log_length != 0)
      {
        stages[i].err = PushArray(RENDER->arena, u8, info_log_length+1);
        glGetShaderInfoLog(stages[i].out, info_log_length, 0, (char *)stages[i].err);
        TraceLog(LOG_ERROR, stages[i].err);
      }
      else
      {
        if(stages[i].type == GL_VERTEX_SHADER)
        {
          TraceLog(LOG_INFO, "SHADER: [ID %d] Vertex shader compiled successfully", stages[i].out);
        }
        else
        {
          TraceLog(LOG_INFO, "SHADER: [ID %d] Pixel shader compiled successfully", stages[i].out);
        }
      }
    }

    // attach shader stages.
    GLuint program = glCreateProgram();
    for(u32 i = 0; i < ArrayCount(stages); i += 1)
    {
      glAttachShader(program, stages[i].out);
    }

    // bind attribute locations.
    R_AttribsArray inputs = r_shader_input_attribs[k];
    for(u32 i = 0; i < inputs.count; i += 1)
    {
      glBindAttribLocation(program, inputs.v[i].index, inputs.v[i].name);
    }

    // link program and check for errors.
    glLinkProgram(program);
    {
      char *err = null;
      GLint info_log_length = 0;
      GLint status = 0;
      glGetProgramiv(program, GL_LINK_STATUS, &status);
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
      if(info_log_length != 0)
      {
        err = PushArray(RENDER->arena, u8, info_log_length+1);
        glGetProgramInfoLog(program, info_log_length, 0, err);
        TraceLog(LOG_ERROR, err);
      }
      else
      {
        TraceLog(LOG_INFO, "SHADER: [ID %d] Program shader loaded successfully", program);
      }
    }
    RENDER->shaders[k] = program;
  }

  glGenVertexArrays(1, &RENDER->vao);
  glBindVertexArray(RENDER->vao);
  glGenBuffers(1, &RENDER->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, RENDER->vbo);
  glBufferData(GL_ARRAY_BUFFER, MB(2), 0, GL_DYNAMIC_DRAW);
}

static void R_Quit(void)
{
  ArenaRelease(RENDER->arena);
}

static void R_DrawAll(R_PassArray *passes)
{ 
  for(u32 passIdx = 0; passIdx < passes->count; passIdx += 1)
  {
    R_Pass *pass = &passes->v[passIdx];
    switch(pass->type)
    {
      case R_PassType_Geo2D: {
        GLuint program = RENDER->shaders[R_PassType_Geo2D];
        rlDrawRenderBatchActive();
        glBindVertexArray(RENDER->vao);
        glUseProgram(program);
        {
          R_PassParams_Geo2D *params = &pass->params_geo_2d;
          R_BatchGroup2DList *groups = &params->batch_groups;
          for(R_BatchGroup2DNode *n = groups->first; n != null; n = n->next)
          {
            R_AttribsArray inputs = r_shader_input_attribs[R_PassType_Geo2D];

            // bind static buffer and set vertex attribs
            glBindBuffer(GL_ARRAY_BUFFER, n->params.mesh_vertices);
            {
              glEnableVertexAttribArray(inputs.v[0].index);
              glVertexAttribDivisor(inputs.v[0].index, 0);
              glVertexAttribPointer(inputs.v[0].index, inputs.v[0].count, inputs.v[0].type, GL_FALSE, sizeof(Vector2), null);
            }

            // stream dynamic data to buffer (per-instance data).
            R_BatchList *batches = &n->batches;
            {
              u32 offset = 0;
              glBindBuffer(GL_ARRAY_BUFFER, RENDER->vbo);
              for(R_BatchNode *batch_node = batches->first; batch_node != null; batch_node = batch_node->next)
              {
                glBufferSubData(GL_ARRAY_BUFFER, offset, batch_node->v.batch_size, batch_node->v.bytes);
                offset += batch_node->v.batch_size;
              }
            } 

            // set model matrix attribs
            for(u32 col = 0; col < 4; col += 1)
            {
              GLuint loc = inputs.v[1].index + col; // base location (1) + column offset
              glEnableVertexAttribArray(loc);
              glVertexAttribDivisor(loc, 1);
              glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, sizeof(R_Hull2DInst), (void*)(sizeof(f32) * 4 * col));
            }

            // set shader uniforms.
            {
              u32 view_loc = glGetUniformLocation(program, "u_view");
              glUniformMatrix4fv(view_loc, 1, GL_FALSE, MatrixToFloat(params->view));

              u32 proj_loc = glGetUniformLocation(program, "u_proj");
              glUniformMatrix4fv(proj_loc, 1, GL_FALSE, MatrixToFloat(params->proj));
            }

            u32 vert_count = n->params.vert_count;
            u32 inst_count = batches->batch_total_size / batches->inst_size;
            glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, vert_count, inst_count);
          }
        }
        glUseProgram(0);
        glBindVertexArray(0);
      } break;
    }
  }
}
