#pragma once

#include "figine/core/camera.hpp"
#include "figine/core/object.hpp"
#include "figine/global.hpp"
#include "figine/logging.hpp"

#include <glm/gtx/string_cast.hpp>

namespace cs7gv5::ass2_3 {

class virtual_bone_t {

public:
  inline virtual_bone_t(glm::vec3 from, glm::vec3 to)
      : transform(glm::mat4(1.0f)) {
    set_transform(from, to);
  }

  const glm::vec3 rotate_ref = {1.0f, 0.0f, 0.0f};
  glm::mat4 transform;

  float length;

  inline void set_transform(const glm::vec3 &from, const glm::vec3 &to) {
    glm::vec3 link = to - from;
    length = glm::length(link);

    glm::vec3 cross = glm::cross(rotate_ref, link);
    glm::quat direction;
    direction.x = cross.x;
    direction.y = cross.y;
    direction.z = cross.z;
    direction.w = (float)(sqrt(pow(glm::length(rotate_ref), 2) *
                               pow(glm::length(link), 2)) +
                          glm::dot(link, rotate_ref));
    direction = glm::normalize(direction);

    glm::mat4 T = glm::translate(glm::mat4(1.0f), 3.25f * from);

    glm::mat4 PR = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), link);
    glm::mat4 R = glm::mat4(direction);

    transform = T * PR * R;
  }
};

} // namespace cs7gv5::ass2_2