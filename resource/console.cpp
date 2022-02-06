#include <console.hpp>
#include <global.hpp>

void cs7gv5::windows::console_t::refresh() {
  ImGui::Begin("cs7gv5 console");

  ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);

  ImGui::Checkbox("Enable Quaternion", &_quaternion);
  ImGui::SliderFloat("Roll", &_roll, -180.0f, 180.0f);
  ImGui::SliderFloat("Pitch", &_pitch, -180.0f, 180.0f);
  ImGui::SliderFloat("Yaw", &_yaw, -180.0f, 180.0f);

  if (ImGui::Button("reset")) {
    _roll = 0;
    _pitch = 0;
    _yaw = 0;
    _quaternion = false;
  }
  ImGui::SameLine();
  if (ImGui::Button("test gimbal lock")) {
    _quaternion = true;
    _roll = 0;
    _pitch = -90;
    _yaw = 0;
  }
  if (ImGui::Button("1st/3rd person view switch")) {
  }

  ImGui::End();
}

cs7gv5::windows::console_t cs7gv5::global::console;
