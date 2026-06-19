#version 330 core

layout (location = 0) in vec2 in_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main(void)
{
  gl_Position = proj*view*model*vec4(in_pos, 0.0, 1.0);
}
