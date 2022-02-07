#pragma once

#include "airplane_model.hpp"
#include "console.hpp"

namespace cs7gv5::global {

extern cs7gvx_utils::gl::camera_t camera_1st;
extern cs7gvx_utils::gl::camera_t camera_3rd;
extern cs7gvx_utils::gl::shader_t phong_shader;
extern cs7gvx_utils::gl::shader_t font_shader;
extern cs7gvx_utils::gl::shader_t naive_shader;
extern windows::console_t console;

extern models::airplane_t airplane;

inline cs7gvx_utils::gl::model_t cube("model/cube/cube.obj",
                                      &cs7gv5::global::naive_shader,
                                      &cs7gv5::global::camera_3rd);

} // namespace cs7gv5::global
