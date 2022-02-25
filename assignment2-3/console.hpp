#pragma once

#include "figine/figine.hpp"

namespace cs7gv5::ass2_3 {

class console_t final : public figine::imnotgui::window_t {
public:
  void refresh() override;

  float x = 0;
  float y = 0;
  float z = 0;
  bool doing = false;
  float anime_duration = 0;
  glm::vec3 direction = glm::vec3(0.0);
};

} // namespace cs7gv5::ass2_3
