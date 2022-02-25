#pragma once

#include "figine/builtin/shader.hpp"
#include "figine/core/shader.hpp"

namespace cs7gv5::ass2_2 {

constexpr uint8_t peter_vs[] = R"(
#version 330 core

layout(location = 0) in vec3 pos_in;
layout(location = 1) in vec3 normal_in;
layout(location = 2) in vec2 texture_coordinate_in;
layout(location = 5) in ivec4 bone_ids_in; 
layout(location = 6) in vec4 weights_in;

out vec3 frag_pos;
out vec3 normal;
out vec2 texture_coordinate;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 bones_matrices[MAX_BONES];
uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 position = vec4(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++) {
        if(bone_ids_in[i] == -1) {
            continue;
        }

        if(bone_ids_in[i] >= MAX_BONES) {
            position = vec4(pos_in,1.0f);
            break;
        }

        vec4 local_position = bones_matrices[bone_ids_in[i]] * vec4(pos_in,1.0f);
        position += local_position * weights_in[i];
        vec3 local_norm = mat3(bones_matrices[bone_ids_in[i]]) * normal_in;
    }
    texture_coordinate = texture_coordinate_in;
    frag_pos = vec3(transform * position);
    normal = mat3(transpose(inverse(transform))) * normal_in;

    gl_Position = projection * view * transform * position;
}
)";

constexpr uint8_t peter_fs[] = R"(
#version 330 core

struct material_t {
    float shininess;
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
};

struct light_t {
    vec3 position;
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
};

in vec3 frag_pos;
in vec3 normal;
in vec2 texture_coordinate;

out vec4 frag_color;

uniform vec3 view_pos;
uniform material_t material;
uniform light_t light;
uniform sampler2D texture_diffuse1;

void main() {
    // vec3 norm = normalize(normal);
    // vec3 view_direction = normalize(view_pos - frag_pos);
    // vec3 light_direction = normalize(frag_pos - light.position);
    // vec3 reflect_direction = reflect(light_direction, norm);

    // // ambient
    // vec3 ambient = light.ambient_color * material.ambient_color;

    // // diffuse 
    // float diff = max(dot(norm, -light_direction), 0.0);
    // vec3 diffuse = diff * light.diffuse_color * material.diffuse_color;

    // // specular
    // float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
    // vec3 specular = spec * light.specular_color * material.specular_color;

    // frag_color = vec4((ambient + diffuse + specular) * texture(texture_diffuse1, texture_coordinate).xyz, 1.0);
     frag_color = texture(texture_diffuse1, texture_coordinate);
}
)";

class peter_shader_t final : public figine::core::shader_if {
public:
  peter_shader_t() : figine::core::shader_if(peter_vs, peter_fs) {}
};

} // namespace cs7gv5::ass2_2
