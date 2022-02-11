#include <resource_index.hpp>

namespace {

cs7gvx_utils::gl::light_t light = {
    .position = {0.0f, 2.0f, 8.0f},
    .diffuse_color = cs7gvx_utils::gl::white * glm::vec3(0.5f),
    .ambient_color = cs7gvx_utils::gl::white,
    .specular_color = glm::vec3(1.0f),
};

cs7gvx_utils::gl::material_t material = {
    .shininess = 16,
    .ambient_color = cs7gvx_utils::gl::gray,
    .diffuse_color = cs7gvx_utils::gl::gray,
    .specular_color = cs7gvx_utils::gl::gray};

} // namespace

cs7gvx_utils::gl::shader_t cs7gv5::global::phong_shader(
    "shader/base.vs", "shader/phong.fs",
    std::make_shared<cs7gvx_utils::gl::phong_profile_t>(material, light));

cs7gvx_utils::gl::shader_t cs7gv5::global::font_shader("shader/font.vs",
                                                       "shader/font.fs");

cs7gvx_utils::gl::shader_t cs7gv5::global::naive_shader(
    "shader/base.vs", "shader/naive.fs",
    std::make_shared<cs7gvx_utils::gl::phong_profile_t>());

cs7gvx_utils::gl::shader_t cs7gv5::global::skybox_shader(
    "shader/skybox.vs", "shader/skybox.fs",
    std::make_shared<cs7gvx_utils::gl::phong_profile_t>());