#pragma once

#include "figine/figine.hpp"
#include "resource.hpp"

namespace cs7gv5::models {

class airplane_t : public figine::core::object_t {
public:
  airplane_t(const glm::vec3 &init_pos, figine::core::shader_if *shader,
             figine::core::camera_t *camera, bool gamma_correction = false);

  void init() override;
  void update() override;

private:
  glm::vec3 _init_pos;
  float roll = 0, pitch = 0, yaw = 0;
};

} // namespace cs7gv5::models
