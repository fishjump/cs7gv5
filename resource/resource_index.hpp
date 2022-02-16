#pragma once

#include "airplane_model.hpp"
#include "console.hpp"

namespace cs7gv5::global {

inline figine::builtin::shader::light_t light = {
    .position = {0.0f, 2.0f, 8.0f},
    .diffuse_color = glm::vec3(1) * glm::vec3(0.5f),
    .ambient_color = glm::vec3(1),
    .specular_color = glm::vec3(1.0f),
};

inline figine::builtin::shader::material_t material = {
    .shininess = 16,
    .ambient_color = glm::vec3(1),
    .diffuse_color = glm::vec3(1),
    .specular_color = glm::vec3(1),
};

inline figine::core::camera_t camera_1st({0.0f, 3.0f, 0.0f});
inline figine::core::camera_t camera_3rd({0.0f, 1.0f, 8.0f});

inline figine::builtin::shader::phong_shader_t phong_shader(material, light);
inline figine::builtin::shader::naive_shader_t naive_shader;
inline figine::builtin::shader::skybox_shader_t skybox_shader;

extern windows::console_t console;

inline cs7gv5::models::airplane_t airplane({0, 0, 0},
                                           &cs7gv5::global::phong_shader,
                                           &cs7gv5::global::camera_3rd);

inline figine::builtin::object::skybox_t
    skybox({"model/skybox/right.jpg", "model/skybox/left.jpg",
            "model/skybox/top.jpg", "model/skybox/bottom.jpg",
            "model/skybox/front.jpg", "model/skybox/back.jpg"},
           &cs7gv5::global::skybox_shader, &cs7gv5::global::camera_3rd);

inline void init() {
  cs7gv5::global::phong_shader.build();
  cs7gv5::global::skybox_shader.build();
  cs7gv5::global::naive_shader.build();

  cs7gv5::global::airplane.init();
  cs7gv5::global::skybox.init();
}

} // namespace cs7gv5::global
