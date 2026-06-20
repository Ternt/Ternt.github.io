// 2026-06-19

const char r_geo2d_vs_shader_src[] = 
{
#if defined(GRAPHICS_API_OPENGL_33)
"#version 330 core\n"
"in vec2 in_pos;                    \n"
"in mat4 in_model;                  \n"
"uniform mat4 u_view;               \n"
"uniform mat4 u_proj;               \n"
#endif

#if defined(GRAPHICS_API_OPENGL_ES3)
"#version 300 es                    \n"
"precision mediump float;           \n"
"in vec2 in_pos;                    \n"
"in mat4 in_model;                  \n"
"uniform mat4 u_view;               \n"
"uniform mat4 u_proj;               \n"
#endif

"void main(void)                    \n"
"{\n"
"  gl_Position = u_proj*u_view*in_model*vec4(in_pos, 0.0, 1.0);\n"
"}                                  \n"
};

const char r_geo2d_ps_shader_src[] = 
{
#if defined(GRAPHICS_API_OPENGL_33)
"#version 330 core                  \n"
"out vec4 frag_color;               \n"
#endif

#if defined(GRAPHICS_API_OPENGL_ES3)
"#version 300 es                    \n"
"precision mediump float;           \n"
"out vec4 frag_color;               \n"
#endif

"void main()                        \n"
"{                                  \n"
"  frag_color = vec4(1.0, 1.0, 1.0, 1.0); \n"
"}                                  \n"
};

const struct { const char *data; u32 size; } r_vs_shader_src[R_PassType_COUNT] = {
  { r_geo2d_vs_shader_src, sizeof(r_geo2d_vs_shader_src) },
};

const struct { const char *data; u32 size; } r_ps_shader_src[R_PassType_COUNT] = {
  { r_geo2d_ps_shader_src, sizeof(r_geo2d_ps_shader_src) },
};
