#include <gl/shader_profile.hpp>

gl::shader_profile_t::shader_profile_t() {}
gl::shader_profile_t::shader_profile_t(const material_t &material,
                                       const light_t &light)
    : material(material), light(light) {}

gl::phong_profile_t::phong_profile_t() {}
gl::phong_profile_t::phong_profile_t(const material_t &material,
                                     const light_t &light)
    : shader_profile_t(material, light) {}

gl::gooch_profile_t::gooch_profile_t() {}
gl::gooch_profile_t::gooch_profile_t(const material_t &material,
                                     const light_t &light)
    : shader_profile_t(material, light) {}

const gl::meta_profile_t &gl::shader_profile_t::meta() {
  gl::insert(_meta, "view_pos", view_pos);

  gl::insert(_meta, "light.position", light.position);
  gl::insert(_meta, "light.ambient_color", light.ambient_color);
  gl::insert(_meta, "light.diffuse_color", light.diffuse_color);
  gl::insert(_meta, "light.specular_color", light.specular_color);

  gl::insert(_meta, "material.shininess", material.shininess);
  gl::insert(_meta, "material.ambient_color", material.ambient_color);
  gl::insert(_meta, "material.diffuse_color", material.diffuse_color);
  gl::insert(_meta, "material.specular_color", material.specular_color);

  gl::insert(_meta, "projection", projection);
  gl::insert(_meta, "view", view);
  gl::insert(_meta, "model", model);

  return _meta;
}

const gl::meta_profile_t &gl::phong_profile_t::meta() {
  shader_profile_t::meta();

  return _meta;
}

const gl::meta_profile_t &gl::gooch_profile_t::meta() {
  shader_profile_t::meta();

  gl::insert(_meta, "a", a);
  gl::insert(_meta, "b", b);
  gl::insert(_meta, "k_blue", k_blue);
  gl::insert(_meta, "k_yellow", k_yellow);

  return _meta;
}

const gl::meta_profile_t &gl::cook_torrance_profile_t::meta() {
  shader_profile_t::meta();

  return _meta;
}