#pragma once

#include "figine/figine.hpp"

namespace cs7gv5::ass3 {

constexpr uint8_t target_vs[] = R"(
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 norm;

out vec3 normal;
out vec3 frag_pos;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view *  transform * vec4(position, 1.0f);
    frag_pos = vec3(transform * vec4(position, 1.0f));
    normal = mat3(transpose(inverse(transform))) * norm;  
}
)";

constexpr uint8_t target_fs[] = R"(
#version 330 core
out vec4 color;

in vec3 frag_pos;  
in vec3 normal;  
  
uniform vec3 light_pos; 
uniform vec3 view_pos;
uniform vec3 light_color;
uniform vec3 object_color;

void main() {
    // ambient
    float ambient_strength = 0.1f;
    vec3 ambient = ambient_strength * light_color;
  	
    // diffuse 
    vec3 norm = normalize(normal);
    vec3 light_direction = normalize(light_pos - frag_pos);
    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = diff * light_color;
    
    // specular
    float specular_strength = 0.5f;
    vec3 view_direction = normalize(view_pos - frag_pos);
    vec3 reflect_direction = reflect(-light_direction, norm);  
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 32);
    vec3 specular = specular_strength * spec * light_color;  
        
    vec3 result = (ambient + diffuse + specular) * object_color;
    color = vec4(result, 1.0f);
} 
)";

class target_obj_t : public figine::core::object_t {
public:
  inline target_obj_t(const glm::vec3 &pos, figine::core::camera_t *camera,
                      bool gamma_correction = false)
      : figine::core::object_t("model/sphere.off", camera, gamma_correction),
        pos(pos) {}

  glm::vec3 pos;

  inline void init() override {
    object_t::init();

    _shader.build();
    transform = translate(pos);
    transform = scale(glm::vec3(0.1f));
  }

  inline void update() override {
    object_t::update();
    transform = translate(pos, glm::mat4(1.0f));
    transform = scale(glm::vec3(0.01f));
  }

  inline void apply_uniform(const figine::core::shader_if &shader) override {
    object_t::apply_uniform(shader);

    shader.set_uniform("object_color", glm::vec3{1.0f, 1.0f, 1.0f});
    shader.set_uniform("light_color", glm::vec3{1.0f, 0.0f, 0.0f});
    shader.set_uniform("light_pos", glm::vec3{1.0f, 1.0f, 3.0f});
    shader.set_uniform("view_pos", glm::vec3{0.0f, 0.0f, 3.0f});
  }

  inline void loop() { object_t::loop(_shader); }

private:
  class target_shader_t final : public figine::core::shader_if {
  public:
    inline target_shader_t() : figine::core::shader_if(target_vs, target_fs) {}
  };

  target_shader_t _shader;
};

} // namespace cs7gv5::ass3
