#include "console.hpp"
#include "resource.hpp"

#include <cmath>

void cs7gv5::ass2_3::console_t::refresh() {
  ImGui::Begin("cs7gv5 console");
  defer(ImGui::End());

  ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);

  ImGui::SliderFloat("X", &x, -1.0f, 1.0f, "%.2f");
  ImGui::SliderFloat("Y", &y, -1.0f, 1.0f, "%.2f");
  ImGui::SliderFloat("Z", &z, -1.0f, 1.0f, "%.2f");

  if (ImGui::Button("reset") && !doing) {
    x = 0;
    y = 0;
    z = 0;
  }

  if (ImGui::Button("preset 1") && !doing) {
    x = 0.188788f;
    y = 0.317575f;
    z = 0.005152f;
  }
  ImGui::SameLine();
  if (ImGui::Button("preset 2") && !doing) {
    x = 0.130000;
    y = 0.500000;
    z = 0.070000;
  }
  ImGui::SameLine();
  if (ImGui::Button("preset 3") && !doing) {
    x = -0.020000;
    y = 0.440000;
    z = 0.100000;
  }

  if (ImGui::Button("do anime") && !doing) {
    direction = glm::vec3(x, y, z) - target.pos;
    anime_duration = 1.0f;
    doing = true;
  }

  if (doing) {
    target.pos += delta_time * direction;
    anime_duration -= delta_time;
    if (anime_duration < 0.01f) {
      doing = false;
    }
  }
}
