#ifndef GL_CAMERA_HPP
#define GL_CAMERA_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

namespace gl {

enum class camera_movement_t { FORWARD, BACKWARD, LEFT, RIGHT };

constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 2.5f;
constexpr float SENSITIVITY = 0.05f;
constexpr float ZOOM = 45.0f;

class camera_t final {
public:
  camera_t(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
           float pitch = PITCH);

  glm::mat4 view_matrix() const;

  void process_keyboard(camera_movement_t direction, float_t delta_time);
  void process_mouse_movement(float x_offset, float y_offset,
                              GLboolean constrain_pitch = true);
  void process_mouse_scroll(float y_offset);

  glm::vec3 position() const;
  glm::vec3 front() const;
  glm::vec3 up() const;
  glm::vec3 right() const;
  glm::vec3 world_up() const;

  float yaw() const;
  float pitch() const;

  float speed() const;
  float mouse_sensitivity() const;
  float zoom() const;

private:
  glm::vec3 _position;
  glm::vec3 _front;
  glm::vec3 _up;
  glm::vec3 _right;
  glm::vec3 _world_up;

  float _yaw;
  float _pitch;

  float _speed;
  float _mouse_sensitivity;
  float _zoom;

  void update_camera_vectors();
};

} // namespace gl

#endif // GL_CAMERA_HPP