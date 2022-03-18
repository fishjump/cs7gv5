#pragma once

#include "figine/figine.hpp"
#include "mesh.h"

#include <eigen3/Eigen/Core>

constexpr uint8_t alien_vs[] = R"(
#version 330 core

layout(location = 0) in vec3 position_in;
layout(location = 1) in vec3 normal_in;

out vec3 frag_pos;
out vec3 normal;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main() {
    frag_pos = vec3(transform * vec4(position_in, 1.0));
    normal = mat3(transpose(inverse(transform))) * normal_in;

    gl_Position = projection * view * vec4(frag_pos, 1.0);
}
)";

constexpr uint8_t alien_fs[] = R"(
#version 330 core

struct material_t {
    float shininess;
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
};

struct light_t {
    vec3 position;
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
};

in vec3 frag_pos;
in vec3 normal;

out vec4 frag_color;

uniform vec3 view_pos;
uniform material_t material;
uniform light_t light;

void main() {
    vec3 norm = normalize(normal);
    vec3 view_direction = normalize(view_pos - frag_pos);
    vec3 light_direction = normalize(frag_pos - light.position);
    vec3 reflect_direction = reflect(light_direction, norm);

    // ambient
    vec3 ambient = light.ambient_color * material.ambient_color;

    // diffuse 
    float diff = max(dot(norm, -light_direction), 0.0);
    vec3 diffuse = diff * light.diffuse_color * material.diffuse_color;

    frag_color = vec4((ambient + diffuse), 1.0);
    // frag_color = vec4(1.0, 1.0, 0.0, 1.0);
}
)";

namespace cs7gv5::ass3 {

extern std::vector<mesh_t> faces;

extern Eigen::VectorXf f0;
extern Eigen::MatrixXf B;
extern Eigen::VectorXf w;
extern Eigen::VectorXf f;

extern float current;
extern float last;
extern float delta;

extern Eigen::VectorXf saved_w;
extern Eigen::VectorXf diff;
extern float animation_duration;
extern bool playing;

class console_t final : public figine::imnotgui::window_t {
public:
  virtual void refresh() final {
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGui::Begin("assignmeng3 console");
    defer(ImGui::End());

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    if (playing) {
      animation_duration -= delta;

      w += diff * delta;

      if (animation_duration < 0.01f) {
        playing = false;
        w = saved_w;
      }

      f = f0 + (B * w);

      return;
    }

    if (ImGui::Button("save")) {
      saved_w = w;
    }

    ImGui::SameLine();

    if (ImGui::Button("play")) {
      playing = true;
      animation_duration = 1.0;
      diff = saved_w - w;
    }

    ImGui::SameLine();

    if (ImGui::Button("reset")) {
      for (int i = 0; i < faces.size(); i++)
        w(i) = 0;
    }

    ImGui::SliderFloat("neutral", &w[0], 0.0f, 1.0f);
    ImGui::SliderFloat("jaw_open", &w[1], 0.0f, 1.0f);
    ImGui::SliderFloat("kiss", &w[2], 0.0f, 1.0f);
    ImGui::SliderFloat("l_brow_lower", &w[3], 0.0f, 1.0f);
    ImGui::SliderFloat("l_brow_narrow", &w[4], 0.0f, 1.0f);
    ImGui::SliderFloat("l_brow_raise", &w[5], 0.0f, 1.0f);
    ImGui::SliderFloat("l_eye_closed", &w[6], 0.0f, 1.0f);
    ImGui::SliderFloat("l_eye_lower_open", &w[7], 0.0f, 1.0f);
    ImGui::SliderFloat("l_eye_upper_open", &w[8], 0.0f, 1.0f);
    ImGui::SliderFloat("l_nose_wrinkle", &w[9], 0.0f, 1.0f);
    ImGui::SliderFloat("l_puff", &w[10], 0.0f, 1.0f);
    ImGui::SliderFloat("l_sad", &w[11], 0.0f, 1.0f);
    ImGui::SliderFloat("l_smile", &w[12], 0.0f, 1.0f);
    ImGui::SliderFloat("l_suck", &w[13], 0.0f, 1.0f);
    ImGui::SliderFloat("r_brow_lower", &w[14], 0.0f, 1.0f);
    ImGui::SliderFloat("r_brow_narrow", &w[15], 0.0f, 1.0f);
    ImGui::SliderFloat("r_brow_raise", &w[16], 0.0f, 1.0f);
    ImGui::SliderFloat("r_eye_closed", &w[17], 0.0f, 1.0f);
    ImGui::SliderFloat("r_eye_lower_open", &w[18], 0.0f, 1.0f);
    ImGui::SliderFloat("r_eye_upper_open", &w[19], 0.0f, 1.0f);
    ImGui::SliderFloat("r_nose_wrinkle", &w[20], 0.0f, 1.0f);
    ImGui::SliderFloat("r_puff", &w[21], 0.0f, 1.0f);
    ImGui::SliderFloat("r_sad", &w[22], 0.0f, 1.0f);
    ImGui::SliderFloat("r_smile", &w[23], 0.0f, 1.0f);
    ImGui::SliderFloat("r_suck", &w[24], 0.0f, 1.0);
  }
};

} // namespace cs7gv5::ass3
