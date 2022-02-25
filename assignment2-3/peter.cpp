#include "figine/figine.hpp"
#include "resource.hpp"

#include <glm/gtx/string_cast.hpp>

namespace cs7gv5::ass2_3 {

inline peter_t::peter_t(const glm::vec3 &init_pos,
                        figine::core::camera_t *camera, bool gamma_correction)
    : figine::core::object_t("model/GV2_Model_Peter/peter.fbx", camera,
                             gamma_correction),
      _init_pos(init_pos) {}

inline void peter_t::init() {
  object_t::init();

  transform = translate(_init_pos);
  transform = scale(glm::vec3(0.3));
}

inline void peter_t::update() { object_t::update(); }

void peter_t::apply_uniform(const figine::core::shader_if &shader) {
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

  auto bones_matrices = animator->bone_matrices;

  for (int i = 0; i < bones_matrices.size(); i++) {
    shader.set_uniform("bones_matrices[" + std::to_string(i) + "]",
                       bones_matrices[i]);
  }
}

}; // namespace cs7gv5::ass2_2
