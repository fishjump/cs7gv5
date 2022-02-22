#pragma once

#include "figine/figine.hpp"
#include "resource.hpp"

namespace cs7gv5::models {

class airplane_t : public figine::core::object_t {
public:
  airplane_t(const glm::vec3 &init_pos, const figine::core::camera_t *camera,
             bool gamma_correction = false);

  figine::builtin::shader::material_t material;
  figine::builtin::shader::light_t light;

  GLfloat a = 0.2;
  GLfloat b = 0.6;
  glm::vec3 k_blue = {0.0f, 0.0f, 0.4f};
  glm::vec3 k_yellow = {0.4f, 0.4f, 0.0f};

  void init() override;
  void update() override;
  void apply_uniform(const figine::core::shader_if &shader) override;

private:
  const glm::vec3 _init_pos;
  float _roll = 0, _pitch = 0, _yaw = 0;
};

} // namespace cs7gv5::models
