#include <console.hpp>
#include <resource_index.hpp>

void cs7gv5::windows::console_t::refresh() {
  ImGui::Begin("cs7gv5 console");

  ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);

  ImGui::Checkbox("Enable Quaternion", &_quaternion);
  ImGui::SliderFloat("Roll", &_roll, -180.0f, 180.0f);
  ImGui::SliderFloat("Pitch", &_pitch, -180.0f, 180.0f);
  ImGui::SliderFloat("Yaw", &_yaw, -180.0f, 180.0f);

  if (ImGui::Button("reset")) {
    _quaternion = true;
    _roll = 0;
    _pitch = 0;
    _yaw = 0;
  }
  ImGui::SameLine();
  if (ImGui::Button("test gimbal lock")) {
    _quaternion = false;
    _roll = 0;
    _pitch = -90;
    _yaw = 0;
  }

  if (ImGui::Button("1st/3rd person view switch")) {
    if (_3rd_person_view) {
      _3rd_person_view = false;
    } else {
      _3rd_person_view = true;
    }
  }

  ImGui::End();
}

cs7gv5::windows::console_t cs7gv5::global::console;
