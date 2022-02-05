#ifndef CS7GV5_IO_HPP
#define CS7GV5_IO_HPP

#include <gl.hpp>

namespace cs7gv5 {

constexpr uint32_t SCR_WIDTH = 800;
constexpr uint32_t SCR_HEIGHT = 600;

void process_input(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double x_pos_in, double y_pos_in);
void scroll_callback(GLFWwindow *window, double x_offset, double y_offset);

const gl::camera_t &camera();
float_t &delta_time();

} // namespace cs7gv5

#endif // CS7GV5_IO_HPP