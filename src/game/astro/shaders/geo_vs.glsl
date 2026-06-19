#version 330 core

layout (location = 0) in vec2 in_pos;
layout (location = 1) in mat4 in_model;

uniform mat4 u_view;
uniform mat4 u_proj;

void main(void)
{
  gl_Position = u_proj*u_view*in_model*vec4(in_pos, 0.0, 1.0);
}
