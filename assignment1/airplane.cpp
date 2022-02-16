#include "resource.hpp"

namespace cs7gv5::models {

inline airplane_t::airplane_t(const glm::vec3 &init_pos,
                              figine::core::shader_if *shader,
                              figine::core::camera_t *camera,
                              bool gamma_correction)
    : figine::core::object_t("model/airplane/piper_pa18.obj", shader, camera,
                             gamma_correction),
      _init_pos(init_pos) {}

inline void airplane_t::init() {
  object_t::init();

  transform = translate(_init_pos);
  transform = scale(glm::vec3(0.05));
}

inline void airplane_t::update() {
  transform =
      rotate(glm::vec3{glm::radians(roll - cs7gv5::global::console.roll),
                       glm::radians(pitch - cs7gv5::global::console.pitch),
                       glm::radians(yaw - cs7gv5::global::console.yaw)},
             global::console.quaternion);

  roll = cs7gv5::global::console.roll;
  pitch = cs7gv5::global::console.pitch;
  yaw = cs7gv5::global::console.yaw;
}

}; // namespace cs7gv5::models
