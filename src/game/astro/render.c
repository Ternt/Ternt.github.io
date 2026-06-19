// 2026-06-17

////////////////////////////
//- Default Shader Info Tables

const char *r_vs_shader_src_path[R_PassType_COUNT] = 
{
  "../astro/shaders/geo_vs.glsl",
};

const char *r_ps_shader_src_path[R_PassType_COUNT] = 
{
  "../astro/shaders/geo_ps.glsl",
};

const R_Attribs r_geo_shader_input_attribs[] = 
{
  { 0, "in_pos", GL_FLOAT, 2 },
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

static R_Handle R_BufferFromSize(u32 size)
{
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
    struct {GLenum type; char *path; GLuint out; char *err;} stages[] =
    {
      {GL_VERTEX_SHADER,   (char*)r_vs_shader_src_path[k]},
      {GL_FRAGMENT_SHADER, (char*)r_ps_shader_src_path[k]},
    };

    // compile shaders stages.
    for(u32 i = 0; i < ArrayCount(stages); i += 1)
    {
      GLint src_size = 0;
      char *src_data = LoadFileData(stages[i].path, &src_size);
      stages[i].out = glCreateShader(stages[i].type);
      glShaderSource(stages[i].out, 1, (char**)&src_data, &src_size);
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
      glBindAttribLocation(program, inputs.data[i].index, inputs.data[i].name);
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
    RENDER->program = program;
  }

  glGenVertexArrays(1, &RENDER->vao);
  glBindVertexArray(RENDER->vao);
  glGenBuffers(1, &RENDER->vbo_64kb);
  glBindBuffer(GL_ARRAY_BUFFER, RENDER->vbo_64kb);
  glBufferData(GL_ARRAY_BUFFER, KB(64), 0, GL_DYNAMIC_DRAW);
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
        rlDrawRenderBatchActive();
        glBindVertexArray(RENDER->vao);
        glUseProgram(RENDER->shaders[R_PassType_Geo2D]);
        {
          R_PassParams_Geo2D params = pass->paramsGeo2D;
          R_BatchGroup2DList groups = params.batchGroup;
          for(R_BatchGroup2DNode *n = groups.first; n; n = n->next)
          {
            // // upload data to buffer.
            // {
            //   u32 offset = 0;
            //   glBindBuffer(GL_ARRAY_BUFFER, RENDER->vbo_64kb);
            //   for(R_BatchNode *n = batches->first; n; n = n->next)
            //   {
            //     glBufferSubData(GL_ARRAY_BUFFER, offset, n->v.batch_size, n->v.bytes);
            //     offset += n->v.batch_size;
            //   }
            // }
            //
            // // set input vertex attributes.
            // for(u32 i = 0; i < inputs.count; i += 1)
            // {
            //   glEnableVertexAttribArray(inputs.data[i].index);
            //   glVertexAttribPointer(inputs.data[i].index, inputs.data[i].size, inputs.data[i].type, GL_FALSE, sizeof(Vector2), null);
            // }
            //
            // // set shader uniforms.
            // {
            //   int w = GetScreenWidth();
            //   int h = GetScreenHeight();
            //
            //   Matrix model = params.;
            //   u32 model_loc = glGetUniformLocation(RENDER->program, "model");
            //   glUniformMatrix4fv(model_loc, 1, GL_FALSE, MatrixToFloat(model));
            //
            //   Matrix view = GetCameraMatrix2D(g_camera);
            //   u32 view_loc = glGetUniformLocation(RENDER->program, "view");
            //   glUniformMatrix4fv(view_loc, 1, GL_FALSE, MatrixToFloat(view));
            //
            //   Matrix proj = MatrixOrtho(0, w, h, 0, 0.0f, 10.0f);
            //   u32 proj_loc = glGetUniformLocation(RENDER->program, "proj");
            //   glUniformMatrix4fv(proj_loc, 1, GL_FALSE, MatrixToFloat(proj));
            }

            // glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, sides, ngons_array->count);
          }
        }
        glUseProgram(0);
        glBindVertexArray(0);
      } break;
    }
  }
}
