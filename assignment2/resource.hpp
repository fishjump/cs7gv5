#pragma once

#include "peter.hpp"
#include "peter_shader.hpp"

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

inline figine::core::camera_t camera({0.0f, 0.3f, 0.75f});

inline cs7gv5::shaders::peter_shader_t peter_shader;
inline figine::builtin::shader::naive_shader_t naive_shader;

inline cs7gv5::models::peter_t peter({0, 0, 0}, &cs7gv5::global::camera);

inline figine::builtin::object::skybox_t
    skybox({"model/skybox/right.jpg", "model/skybox/left.jpg",
            "model/skybox/top.jpg", "model/skybox/bottom.jpg",
            "model/skybox/front.jpg", "model/skybox/back.jpg"},
           &cs7gv5::global::camera);

inline void init() {
  cs7gv5::global::peter_shader.build();
  cs7gv5::global::naive_shader.build();

  cs7gv5::global::peter.init();
  cs7gv5::global::skybox.init();
}

} // namespace cs7gv5::global
