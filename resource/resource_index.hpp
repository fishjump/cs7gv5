#pragma once

#include "airplane_model.hpp"
#include "console.hpp"
#include "skybox.hpp"

namespace cs7gv5::global {

extern cs7gvx_utils::gl::camera_t camera_1st;
extern cs7gvx_utils::gl::camera_t camera_3rd;

extern cs7gvx_utils::gl::shader_t phong_shader;
extern cs7gvx_utils::gl::shader_t font_shader;
extern cs7gvx_utils::gl::shader_t naive_shader;
extern cs7gvx_utils::gl::shader_t skybox_shader;

extern windows::console_t console;

extern models::airplane_t airplane;
extern models::skybox_t skybox;
extern cs7gvx_utils::gl::model_t cube;

inline void init() {
  cs7gv5::global::phong_shader.build();
  cs7gv5::global::font_shader.build();
  cs7gv5::global::skybox_shader.build();
  cs7gv5::global::naive_shader.build();

  cs7gv5::global::airplane.init();
  cs7gv5::global::skybox.init();
  cs7gv5::global::cube.init();
}

} // namespace cs7gv5::global
