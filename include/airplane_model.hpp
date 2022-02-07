#pragma once

#include <cs7gvx_utils.hpp>

namespace cs7gv5::models {

class airplane_t : public cs7gvx_utils::gl::model_t {
public:
  airplane_t(const glm::vec3 &init_pos, cs7gvx_utils::gl::shader_t *shader,
             cs7gvx_utils::gl::camera_t *camera,
             float aspect_ratio = (float)800 / (float)600,
             bool gamma_correction = false);

  void init() override;
  void update() override;

private:
  glm::vec3 _init_pos;
};

} // namespace cs7gv5::models
