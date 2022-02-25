#pragma once

#include "figine/core/camera.hpp"
#include "figine/core/object.hpp"
#include "figine/global.hpp"
#include "figine/logging.hpp"

#include <glm/gtx/string_cast.hpp>

namespace cs7gv5::ass2_1 {

constexpr uint8_t bone_vs[] = R"(
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 norm;

out vec3 normal;
out vec3 frag_pos;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view *  transform * vec4(position, 1.0f);
    frag_pos = vec3(transform * vec4(position, 1.0f));
    normal = mat3(transpose(inverse(transform))) * norm;  
}
)";

constexpr uint8_t bone_fs[] = R"(
#version 330 core
out vec4 color;

in vec3 frag_pos;  
in vec3 normal;  
  
uniform vec3 light_pos; 
uniform vec3 view_pos;
uniform vec3 light_color;
uniform vec3 object_color;

void main() {
    // ambient
    float ambient_strength = 0.1f;
    vec3 ambient = ambient_strength * light_color;
  	
    // diffuse 
    vec3 norm = normalize(normal);
    vec3 light_direction = normalize(light_pos - frag_pos);
    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = diff * light_color;
    
    // specular
    float specular_strength = 0.5f;
    vec3 view_direction = normalize(view_pos - frag_pos);
    vec3 reflect_direction = reflect(-light_direction, norm);  
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 32);
    vec3 specular = specular_strength * spec * light_color;  
        
    vec3 result = (ambient + diffuse + specular) * object_color;
    color = vec4(result, 1.0f);
} 
)";

class bone_obj_t : public figine::core::object_if {

public:
  inline bone_obj_t(glm::vec3 from, glm::vec3 to,
                    const figine::core::camera_t *camera,
                    bool gamma_correction = false)
      : figine::core::object_if(camera), gamma_correction(gamma_correction) {
    set_transform(from, to);
  }

  const glm::vec3 rotate_ref = {0.0f, 0.0f, -1.0f};

  float length;

  bool gamma_correction;

  inline void set_transform(const glm::vec3 &from, const glm::vec3 &to) {
    glm::vec3 link = to - from;
    glm::vec3 cross = glm::cross(rotate_ref, link);
    length = glm::length(link);
    glm::quat direction;
    direction.x = cross.x;
    direction.y = cross.y;
    direction.z = cross.z;
    direction.w = (float)(sqrt(pow(glm::length(rotate_ref), 2) *
                               pow(glm::length(link), 2)) +
                          glm::dot(link, rotate_ref));
    direction = glm::normalize(direction);

    glm::mat4 T = glm::translate(glm::mat4(1.0f), from);
    glm::mat4 PT = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -0.5));
    glm::mat4 PS = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 1.0f));
    glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, length));
    glm::mat4 R = glm::mat4(direction);

    transform = T * R * S * PT * PS;
  }

  inline void init() override {
    _shader.build();

    const float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  -0.5f, -0.5f,
        0.0f,  0.0f,  -1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
        0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, 0.5f,  -0.5f,
        0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,

        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,
        0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, 0.5f,  0.5f,
        0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f,
        -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,
        -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
        1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
        0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, 0.5f,
        1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, -0.5f,
        0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
        0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, 0.5f,
        0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,

        -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
        0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,
        0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f};

    // model
    GLuint vbo = 0;
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)0);
    // norm
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
  }

  inline void update() override {
    view_pos = camera->position;
    projection =
        glm::perspective(glm::radians(camera->zoom),
                         figine::global::win_mgr::aspect_ratio(), 0.1f, 100.0f);
    view = camera->view_matrix();
  }

  inline void loop() { loop(_shader); }

  inline void loop(const figine::core::shader_if &shader) override {
    update();
    apply_uniform(shader);

    glBindVertexArray(_vao);
    defer(glBindVertexArray(0));

    glDrawArrays(GL_TRIANGLES, 0, 36);
  }

  inline void apply_uniform(const figine::core::shader_if &shader) override {
    object_if::apply_uniform(shader);

    shader.set_uniform("object_color", glm::vec3{1.0f, 1.0f, 1.0f});
    shader.set_uniform("light_color", glm::vec3{1.0f, 1.0f, 1.0f});
    shader.set_uniform("light_pos", glm::vec3{1.0f, 1.0f, 3.0f});
    shader.set_uniform("view_pos", glm::vec3{0.0f, 0.0f, 3.0f});
  }

private:
  class bone_shader_t final : public figine::core::shader_if {
  public:
    inline bone_shader_t() : figine::core::shader_if(bone_vs, bone_fs) {}
  };

  bone_shader_t _shader;
  GLuint _vao = 0, _box_texture = 0;
};

} // namespace cs7gv5::ass2_1