#version 330 core

layout(location = 0) in vec3 pos_in;
layout(location = 1) in vec3 normal_in;
layout(location = 2) in vec2 texture_coordinate_in;

out vec3 frag_pos;
out vec3 normal;
out vec2 texture_coordinate;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    texture_coordinate = texture_coordinate_in;
    frag_pos = vec3(model * vec4(pos_in, 1.0));
    normal = mat3(transpose(inverse(model))) * normal_in;

    gl_Position = projection * view * vec4(frag_pos, 1.0);
}