// 2026-06-17

#ifndef RENDER_H
#define RENDER_H

////////////////////////////
//- Render Resource

typedef struct R_Handle {
  u64 v[0];
} R_Handle;

////////////////////////////
//- Batch Instance Type Definitions

typedef struct R_Batch {
  u8 *bytes;
  u32 batch_size;
  u32 batch_cap;
} R_Batch;

typedef struct R_BatchNode R_BatchNode; 
struct R_BatchNode{
  R_BatchNode *next;
  R_Batch v;
};

typedef struct R_BatchList {
  R_BatchNode *first;
  R_BatchNode *last;
  u32 batch_total_size;
  u32 batch_count;
  u32 inst_size;
} R_BatchList;

typedef struct R_BatchGroup2DParams {
  R_Handle meshVertices;
} R_BatchGroup2DParams;

typedef struct R_BatchGroup2DNode R_BatchGroup2DNode;
struct R_BatchGroup2DNode {
  R_BatchGroup2DNode *next;
  R_BatchList batches;
  R_BatchGroup2DParams params;
};

typedef struct R_BatchGroup2DList {
  R_BatchGroup2DNode *first;
  R_BatchGroup2DNode *last;
  u32 node_count;
} R_BatchGroup2DList;

////////////////////////////
//- Render Pass Type Definitions

typedef s32 R_PassType;
enum 
{
  R_PassType_NULL  = -1,
  R_PassType_Geo2D =  0,
  R_PassType_COUNT,
};

typedef struct R_PassParams_Geo2D {
  Matrix view;
  Matrix proj;
  R_BatchGroup2DList batchGroup;
} R_PassParams_Geo2D;

typedef struct R_Pass {
  R_PassType type;
  union
  {
    void *params;
    R_PassParams_Geo2D paramsGeo2D;
  };
} R_Pass;

typedef struct R_PassArray {
  R_Pass v[R_PassType_COUNT];
  u32 count;
} R_PassArray;

////////////////////////////
//- OpenGL Shader Attributes Type Helpers

typedef struct R_Attribs {
  GLuint index;
  char *name;
  GLenum type;
  GLsizei size;
} R_Attribs;

typedef struct R_AttribsArray {
  const R_Attribs *data; 
  u32 count;
} R_AttribsArray;

////////////////////////////
//- Render Type Definitions

typedef struct R_RenderState {
  Arena *arena;
  GLuint vao;
  GLuint vbo_64kb;
  GLuint shaders[R_PassType_COUNT];
} R_RenderState;

////////////////////////////
//- Render Globals

R_RenderState *RENDER = null;

////////////////////////////
//- Render Functions

static R_Handle R_BufferFromSize(u32 size);
static R_Pass *R_PushPass(R_PassArray *passes, R_PassType type);
static R_BatchList R_MakeBatchList(u32 inst_size);
static void *R_PushBatchInst(Arena *arena, R_BatchList *list, u32 batch_inst_cap);

static void R_Init(void);
static void R_Quit(void);
static void R_DrawAll(R_PassArray *passes);

#endif // RENDER_H
