#include "figine/figine.hpp"
#include "resource.hpp"

#include <glm/gtx/string_cast.hpp>

namespace {} // namespace

namespace cs7gv5::models {

inline peter_t::peter_t(const glm::vec3 &init_pos,
                        figine::core::camera_t *camera, bool gamma_correction)
    : figine::core::object_t("model/vampire/vampire.fbx", camera,
                             gamma_correction),
      _init_pos(init_pos) {}

inline void peter_t::init() {
  object_t::init();

  transform = translate({0, 0, 0});
  transform = scale(glm::vec3(0.003));
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

  static figine::core::animation_t *animation = nullptr;
  static figine::core::animator_t *animator = nullptr;

  if (animation == nullptr) {
    animation = new figine::core::animation_t("model/vampire/vampire.fbx",
                                              cs7gv5::global::peter);
  }

  if (animator == nullptr) {
    animator = new figine::core::animator_t(animation);
  }

  animator->update(.01f);
  auto bones_matrices = animator->bone_matrices;

  for (int i = 0; i < bones_matrices.size(); i++) {
    shader.set_uniform("bones_matrices[" + std::to_string(i) + "]",
                       bones_matrices[i]);
  }
}

}; // namespace cs7gv5::models
