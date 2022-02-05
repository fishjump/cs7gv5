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

// material parameters
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

// lights

const float PI = 3.14159265359;

float DistributionGGX(vec3 norm, vec3 H, float roughness) {
  float a = roughness * roughness;
  float a2 = a * a;
  float NdotH = max(dot(norm, H), 0.0);
  float NdotH2 = NdotH * NdotH;

  float num = a2;
  float denom = (NdotH2 * (a2 - 1.0) + 1.0);
  denom = PI * denom * denom;

  return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
  float r = (roughness + 1.0);
  float k = (r * r) / 8.0;

  float num = NdotV;
  float denom = NdotV * (1.0 - k) + k;

  return num / denom;
}

float GeometrySmith(vec3 norm, vec3 view_direction, vec3 L, float roughness) {
  float NdotV = max(dot(norm, view_direction), 0.0);
  float NdotL = max(dot(norm, L), 0.0);
  float ggx2 = GeometrySchlickGGX(NdotV, roughness);
  float ggx1 = GeometrySchlickGGX(NdotL, roughness);

  return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
  return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main() {
  vec3 norm = normalize(normal);
  vec3 view_direction = normalize(view_pos - frag_pos);
  vec3 light_direction = normalize(light.position - frag_pos);
  vec3 reflect_direction = reflect(-light_direction, norm);

  vec3 F0 = vec3(0.04);
  F0 = mix(F0, albedo, metallic);

  // calculate per-light radiance
  vec3 L = normalize(light.position - frag_pos);
  vec3 H = normalize(view_direction + L);
  float distance = length(light.position - frag_pos);
  float attenuation = 1.0 / (distance * distance);
  vec3 radiance = light.diffuse_color * attenuation;        

  // cook-torrance brdf
  float NDF = DistributionGGX(norm, H, roughness);
  float G = GeometrySmith(norm, view_direction, L, roughness);
  vec3 F = fresnelSchlick(max(dot(H, view_direction), 0.0), F0);

  vec3 kS = F;
  vec3 kD = vec3(1.0) - kS;
  kD *= 1.0 - metallic;

  vec3 numerator = NDF * G * F;
  float denominator = 4.0 * max(dot(norm, view_direction), 0.0) * max(dot(norm, L), 0.0) + 0.0001;
  vec3 specular = numerator / denominator;  

  // add to outgoing radiance Lo
  float NdotL = max(dot(norm, L), 0.0);
  vec3 Lo = (kD * albedo / PI + specular) * radiance * NdotL;

  vec3 ambient = vec3(0.03) * albedo * ao;
  vec3 color = ambient + Lo;

  color = color / (color + vec3(1.0));
  color = pow(color, vec3(1.0 / 2.2));

  FragColor = vec4(color, 1.0);
}