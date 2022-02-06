#pragma once

#include "airplane_model.hpp"
#include "console.hpp"

namespace cs7gv5::global {

extern cs7gvx_utils::gl::camera_t camera;
extern cs7gvx_utils::gl::shader_t phong_shader;
extern cs7gvx_utils::gl::shader_t font_shader;
extern windows::console_t console;
extern models::airplane_t airplane;

} // namespace cs7gv5::global
