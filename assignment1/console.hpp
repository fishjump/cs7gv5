#pragma once

#include "figine/figine.hpp"

namespace cs7gv5::windows {

class console_t final : public figine::imnotgui::window_t {
public:
  void refresh() override;

  bool quaternion = false;
  bool third_person_view = true;
  float roll = 0;
  float pitch = 0;
  float yaw = 0;
};

} // namespace cs7gv5::windows
