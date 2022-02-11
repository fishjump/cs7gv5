#include <resource_index.hpp>

cs7gv5::models::airplane_t::airplane_t(const glm::vec3 &init_pos,
                                       cs7gvx_utils::gl::shader_t *shader,
                                       cs7gvx_utils::gl::camera_t *camera,
                                       float aspect_ratio,
                                       bool gamma_correction)
    : cs7gvx_utils::gl::model_t("model/airplane/piper_pa18.obj", shader, camera,
                                aspect_ratio, gamma_correction),
      _init_pos(init_pos) {}

void cs7gv5::models::airplane_t::init() {
  model_t::init();

  transform = translate(_init_pos);
  transform = scale(glm::vec3(0.3));
}

void cs7gv5::models::airplane_t::update() {
  transform = rotate(glm::vec3{glm::radians(global::console._roll),
                               glm::radians(global::console._pitch),
                               glm::radians(global::console._yaw)},
                     global::console._quaternion, glm::mat4(1));
}

cs7gv5::models::airplane_t
    cs7gv5::global::airplane({0, 0, 0}, &cs7gv5::global::phong_shader,
                             &cs7gv5::global::camera_3rd);
