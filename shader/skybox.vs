#version 330 core

layout(location = 0) in vec3 pos_in;

out vec3 texture_coordinate;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
  texture_coordinate = pos_in;
  vec3 frag_pos = vec3(model * vec4(pos_in, 1.0));

  gl_Position = projection * view * vec4(frag_pos, 1.0);
}