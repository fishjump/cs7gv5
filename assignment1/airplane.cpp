#include "resource.hpp"

namespace cs7gv5::models {

airplane_t::airplane_t(const glm::vec3 &init_pos,
                       const figine::core::camera_t *camera,
                       bool gamma_correction)
    : figine::core::object_t("model/airplane/piper_pa18.obj", camera,
                             gamma_correction),
      _init_pos(init_pos) {}

void airplane_t::init() {
  object_t::init();

  transform = translate(_init_pos);
  transform = scale(glm::vec3(0.05));
}

void airplane_t::update() {
  object_t::update();
  transform =
      rotate(glm::vec3{glm::radians(_roll - cs7gv5::global::console.roll),
                       glm::radians(_pitch - cs7gv5::global::console.pitch),
                       glm::radians(_yaw - cs7gv5::global::console.yaw)},
             global::console.quaternion);

  _roll = cs7gv5::global::console.roll;
  _pitch = cs7gv5::global::console.pitch;
  _yaw = cs7gv5::global::console.yaw;
}

void airplane_t::apply_uniform(const figine::core::shader_if &shader) {
  object_t::apply_uniform(shader);

  shader.set_uniform("light.position", light.position);
  shader.set_uniform("light.ambient_color", light.ambient_color);
  shader.set_uniform("light.diffuse_color", light.diffuse_color);
  shader.set_uniform("light.specular_color", light.specular_color);

  shader.set_uniform("material.shininess", material.shininess);
  shader.set_uniform("material.ambient_color", material.ambient_color);
  shader.set_uniform("material.diffuse_color", material.diffuse_color);
  shader.set_uniform("material.specular_color", material.specular_color);

  shader.set_uniform("a", a);
  shader.set_uniform("b", b);
  shader.set_uniform("k_blue", k_blue);
  shader.set_uniform("k_yellow", k_yellow);
}

}; // namespace cs7gv5::models
