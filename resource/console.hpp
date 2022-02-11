#pragma once

#include <cs7gvx_utils.hpp>

namespace cs7gv5::windows {

class console_t final : public cs7gvx_utils::imnotgui::window_t {
public:
  void refresh() override;

  // private:
  bool _quaternion = false;
  bool _3rd_person_view = true;
  float _roll = 0;
  float _pitch = 0;
  float _yaw = 0;
};

} // namespace cs7gv5::windows
