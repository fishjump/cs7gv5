#pragma once

#include "FABRIK_solver.hpp"
#include "console.hpp"
#include "peter.hpp"
#include "peter_shader.hpp"
#include "target_obj.hpp"

namespace cs7gv5::ass2_3 {

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

inline cs7gv5::ass2_3::peter_shader_t peter_shader;

inline cs7gv5::ass2_3::peter_t peter({0, 0, 0}, &camera);
inline cs7gv5::ass2_3::target_obj_t target({0, 0, 0}, &camera);

inline figine::builtin::object::skybox_t
    skybox({"model/skybox/right.jpg", "model/skybox/left.jpg",
            "model/skybox/top.jpg", "model/skybox/bottom.jpg",
            "model/skybox/front.jpg", "model/skybox/back.jpg"},
           &camera);

inline figine::core::animation_t *animation = nullptr;
inline figine::core::animator_t *animator = nullptr;
inline console_t console;

inline float delta_time = 0;

inline solver_t solver({
    {0.06f, 0.435f, -0.01f},
    {0.124663f, 0.377265f, -0.000762f},
    {0.188788f, 0.317575f, 0.005152f},
});

inline void init(GLFWwindow *window) {
  peter_shader.build();
  peter.init();
  target.init();
  skybox.init();

  figine::imnotgui::init(window);

  figine::imnotgui::register_window(&console);

  if (animation == nullptr) {
    animation =
        new figine::core::animation_t("model/GV2_Model_Peter/peter.fbx", peter);
  }

  if (animator == nullptr) {
    animator = new figine::core::animator_t(animation);
  }
}

} // namespace cs7gv5::ass2_3
