#version 330 core

out vec4 frag_color;

in vec3 texture_coordinate;

uniform samplerCube skybox;

void main() {
  frag_color = texture(skybox, texture_coordinate);
}