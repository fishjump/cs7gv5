#include "bone_obj.hpp"
#include "figine/figine.hpp"

#include <vector>

namespace cs7gv5::ass2_1 {

class solver_t {
public:
  inline solver_t(const std::vector<glm::vec3> &joints,
                  const figine::core::camera_t *camera)
      : joints(joints), camera(camera), total_len(0.0f) {
    if (joints.size() < 2) {
      return;
    }

    for (int i = 0; i < joints.size() - 1; i++) {
      bone_obj_t obj = bone_obj_t(joints[i], joints[i + 1], camera);
      obj.init();
      bones.push_back(std::move(obj));
      total_len += glm::abs(glm::length(joints[i] - joints[i + 1]));
    }
  }

  const float tolerance = 0.01f;
  const int max_iter = 10;

  inline void solve(const glm::vec3 &target) {
    float distance = glm::length(target - joints[0]);

    // no solution, extend all
    if (distance > total_len) {
      for (int i = 0; i < joints.size() - 1; i++) {
        // next joint = current + norm to target * length
        joints[i + 1] =
            joints[i] + glm::normalize(target - joints[i]) * bones[i].length;
      }
    } else {
      int count = 0;
      float diff = glm::length(joints[joints.size() - 1] - target);
      do {
        forward(backward(target));
        diff = glm::length(joints[joints.size() - 1] - target);

        count++;
      } while (count > max_iter && diff > tolerance);
    }

    // apply changes to bones
    for (int i = 0; i < joints.size() - 1; i++) {
      bones[i].set_transform(joints[i], joints[i + 1]);
    }
  }

  inline void forward(const glm::vec3 &origin) {
    joints[0] = origin;
    for (int i = 0; i < joints.size() - 1; i++) {
      // next joint = current joint + norm to next * length
      glm::vec3 direction = joints[i + 1] - joints[i];
      glm::vec3 norm = direction / glm::length(direction);
      joints[i + 1] = joints[i] + bones[i].length * norm;
    }
  }

  inline glm::vec3 backward(const glm::vec3 &target) {
    glm::vec3 origin = joints[0];
    joints[joints.size() - 1] = target;
    for (int i = joints.size() - 2; i >= 0; i--) {
      // current joint = prev joint + norm to current * length
      glm::vec3 direction = joints[i] - joints[i + 1];
      glm::vec3 norm = direction / glm::length(direction);
      joints[i] = joints[i + 1] + bones[i].length * norm;
    }

    return origin;
  }

  inline void draw() {
    for (auto &bone : bones) {
      bone.loop();
    }
  }

private:
  std::vector<glm::vec3> joints;
  std::vector<bone_obj_t> bones;

  const figine::core::camera_t *camera;

  float total_len;
};

} // namespace cs7gv5::ass2_1