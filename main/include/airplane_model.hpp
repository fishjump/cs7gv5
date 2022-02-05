#ifndef CS7GV5_TEAPOT_HPP
#define CS7GV5_TEAPOT_HPP

#include <gl.hpp>

namespace cs7gv5 {

class airplane_t : public gl::model_t {
public:
  airplane_t(const glm::vec3 &init_pos, const gl::shader_t &shader,
             const gl::camera_t *camera, bool gamma_correction = false)
      : gl::model_t("model/teapot.obj", shader, camera, gamma_correction),
        _init_pos(init_pos) {}

  void init() override { translate(_init_pos); }
  void update() override { rotate(1, {0, 1, 0}); }

private:
  glm::vec3 _init_pos;
};

} // namespace cs7gv5

#endif // CS7GV5_TEAPOT_HPP