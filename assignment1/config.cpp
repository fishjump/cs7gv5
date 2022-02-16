#include "config.hpp"
#include "resource.hpp"

namespace {

float last_x = cs7gv5::SCR_WIDTH / 2.0f;
float last_y = cs7gv5::SCR_HEIGHT / 2.0f;
bool first_mouse = true;
float _delta_time = 0.1f;

} // namespace

float_t &cs7gv5::delta_time() { return _delta_time; }

void cs7gv5::process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    cs7gv5::global::camera_3rd.process_keyboard(
        figine::core::camera_movement_t::FORWARD, _delta_time);
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    cs7gv5::global::camera_3rd.process_keyboard(
        figine::core::camera_movement_t::BACKWARD, _delta_time);
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    cs7gv5::global::camera_3rd.process_keyboard(
        figine::core::camera_movement_t::LEFT, _delta_time);
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    cs7gv5::global::camera_3rd.process_keyboard(
        figine::core::camera_movement_t::RIGHT, _delta_time);
  }
}

void cs7gv5::framebuffer_size_callback(GLFWwindow *window, int width,
                                       int height) {
  glViewport(0, 0, width, height);
}

void cs7gv5::mouse_callback(GLFWwindow *window, double x_pos_in,
                            double y_pos_in) {
  float x_pos = static_cast<float>(x_pos_in);
  float y_pos = static_cast<float>(y_pos_in);

  if (first_mouse) {
    last_x = x_pos;
    last_y = y_pos;
    first_mouse = false;
  }

  float x_offset = x_pos - last_x;
  float y_offset = last_y - y_pos;

  last_x = x_pos;
  last_y = y_pos;

  cs7gv5::global::camera_3rd.process_mouse_movement(x_offset, y_offset);
}

void cs7gv5::scroll_callback(GLFWwindow *window, double x_offset,
                             double y_offset) {
  cs7gv5::global::camera_3rd.process_mouse_scroll(static_cast<float>(y_offset));
}