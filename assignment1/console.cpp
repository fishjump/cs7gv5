#include "console.hpp"
#include "resource.hpp"
#include <cmath>

void cs7gv5::windows::console_t::refresh() {
  ImGui::Begin("cs7gv5 console");

  ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);

  ImGui::Checkbox("Enable Quaternion", &quaternion);
  ImGui::SliderFloat("Roll", &roll, -180.0f, 180.0f, "%.0f");
  ImGui::SliderFloat("Pitch", &pitch, -180.0f, 180.0f, "%.0f");
  ImGui::SliderFloat("Yaw", &yaw, -180.0f, 180.0f, "%.0f");

  if (ImGui::Button("reset")) {
    quaternion = true;
    roll = 0;
    pitch = 0;
    yaw = 0;
  }
  ImGui::SameLine();
  if (ImGui::Button("test gimbal lock")) {
    quaternion = false;
    roll = 0;
    pitch = -90;
    yaw = 0;
  }

  if (ImGui::Button("1st/3rd person view switch")) {
    if (third_person_view) {
      third_person_view = false;
    } else {
      third_person_view = true;
    }
  }

  ImGui::End();
}
