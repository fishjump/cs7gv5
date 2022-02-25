#include "FABRIK_solver.hpp"
#include "figine/figine.hpp"
#include "target_obj.hpp"

using namespace cs7gv5::ass2_1;

namespace {

figine::core::camera_t camera({0, 0, 5});
target_obj_t target({1, 1, 0}, &camera);

} // namespace

void process_input(GLFWwindow *window, float delta_time) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    target.pos += glm::vec3{0.0f, 1.0f, 0.0f} * delta_time;
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    target.pos -= glm::vec3{0.0f, 1.0f, 0.0f} * delta_time;
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    target.pos -= glm::vec3{1.0f, 0.0f, 0.0f} * delta_time;
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    target.pos += glm::vec3{1.0f, 0.0f, 0.0f} * delta_time;
  }
}

int main() {
  figine::global::init();

  GLFWwindow *window = figine::global::win_mgr::create_window(
      800, 600, "cs7gv5 - W/A/S/D to move target", NULL, NULL);

  target.init();

  std::vector<glm::vec3> joints;
  for (int i = 0; i < 10; i++) {
    float r = (float)rand() / RAND_MAX;
    joints.push_back(glm::vec3{r, 0.0f, 0.0f});
  }

  solver_t solver(joints, &camera);

  float last_time = 0;
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float current_time = glfwGetTime();
    float delta_time = current_time - last_time;
    last_time = current_time;

    process_input(window, delta_time);

    solver.solve(target.pos);
    solver.draw();
    target.loop();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}