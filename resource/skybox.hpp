#pragma once

#include <cs7gvx_utils.hpp>

namespace cs7gv5::models {

class skybox_t : public cs7gvx_utils::gl::model_if {
public:
  // put skybox faces in this order: right, left, top, bottom, front, back
  skybox_t(const std::array<std::string, 6> &faces);

  void init() override;
  void loop() override;

private:
  GLuint _vao = 0, _box_texture = 0;
  std::array<std::string, 6> _faces;
};

} // namespace cs7gv5::models
