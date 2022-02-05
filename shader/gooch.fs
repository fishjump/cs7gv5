#version 330 core

struct material_t {
  vec3 ambient_color;
  vec3 diffuse_color;
  vec3 specular_color;
  float shininess;
};

struct light_t {
  vec3 position;
  vec3 ambient_color;
  vec3 diffuse_color;
  vec3 specular_color;
};

in vec3 frag_pos;
in vec3 normal;
in vec2 texture_coordinate;

out vec4 frag_color;

uniform vec3 view_pos;
uniform material_t material;
uniform light_t light;
uniform sampler2D texture_diffuse1;

uniform float a;
uniform float b;
uniform vec3 k_blue;
uniform vec3 k_yellow;

vec3 k_cool_f(vec3 k_d) {
  return k_blue + a * k_d;
}

vec3 k_warm_f(vec3 k_d) {
  return k_yellow + b * k_d;
}

void main() {
  vec3 norm = normalize(normal);
  vec3 view_direction = normalize(view_pos - frag_pos);
  vec3 light_direction = normalize(light.position - frag_pos);
  vec3 reflect_direction = reflect(-light_direction, norm);

  // diffuse 
  float diff = dot(-light_direction, norm);
  float dot_1_2 = (1.0 + diff) / 2.0;
  vec3 diffuse_color = diff * light.diffuse_color * material.diffuse_color;
  vec3 k_cool = k_cool_f(diffuse_color);
  vec3 k_warm = k_warm_f(diffuse_color);
  vec3 diffuse = dot_1_2 * k_cool + (1.0 - dot_1_2) * k_warm;

  // specular
  float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
  vec3 specular = spec * light.specular_color * material.specular_color;

  vec3 result = diffuse + specular;
  frag_color = vec4(result, 1.0);
}