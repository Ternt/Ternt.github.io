// 2026-06-17
//
// An immediate mode rendering library.
// Resources are created at the start of
// a frame and destroyed at the end of a
// frame. This provides alot of flexibility,
// allowing things to dynamically change.

#ifndef RENDER_H
#define RENDER_H

////////////////////////////
//- Render Enumerations

typedef enum R_BufferType {
  R_BufferType_Static,
  R_BufferType_Dynamic,
  R_BufferType_Stream,
  R_BufferType_COUNT,
} R_BufferType;

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
  GLuint mesh_vertices;
  u32 vert_count;
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
//- Instance Type Definitions

typedef struct R_Hull2DInst {
  Matrix model;
} R_Hull2DInst;

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
  R_BatchGroup2DList batch_groups;
} R_PassParams_Geo2D;

typedef struct R_Pass {
  R_PassType type;
  union
  {
    void *params;
    R_PassParams_Geo2D params_geo_2d;
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
  GLsizei count;
} R_Attribs;

typedef struct R_AttribsArray {
  const R_Attribs *v; 
  u32 count;
} R_AttribsArray;

////////////////////////////
//- Render Type Definitions

typedef struct R_RenderState {
  Arena *arena;
  GLuint vao;
  GLuint vbo;
  GLuint shaders[R_PassType_COUNT];
} R_RenderState;

////////////////////////////
//- Render Globals

R_RenderState *RENDER = null;

////////////////////////////
//- Render Functions

// resource alloc/release functions
static GLuint R_AllocBuffer(R_BufferType type, u32 size, void *data);
static void R_ReleaseBuffer(GLuint buffer);

// pipeline builder functions
static R_Pass *R_PushPass(R_PassArray *passes, R_PassType type);
static R_BatchList R_MakeBatchList(u32 inst_size);
static void *R_PushBatchInst(Arena *arena, R_BatchList *list, u32 batch_inst_cap);

static void R_Init(void);
static void R_Quit(void);
static void R_DrawAll(R_PassArray *passes);

#endif // RENDER_H
