#include "FABRIK_solver.hpp"
#include "figine/figine.hpp"
#include "resource.hpp"

void process_input(GLFWwindow *window, float delta_time) {
  using namespace cs7gv5::ass2_2;

  delta_time *= 0.05f;

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

  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
    target.pos += glm::vec3{0.0f, 0.0f, 1.0f} * delta_time;
  }

  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
    target.pos -= glm::vec3{0.0f, 0.0f, 1.0f} * delta_time;
  }

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    camera.process_keyboard(figine::core::camera_movement_t::FORWARD,
                            delta_time);
  }

  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    camera.process_keyboard(figine::core::camera_movement_t::BACKWARD,
                            delta_time);
  }

  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    camera.process_keyboard(figine::core::camera_movement_t::LEFT, delta_time);
  }

  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    camera.process_keyboard(figine::core::camera_movement_t::RIGHT, delta_time);
  }
}

int main(int argc, char **argv) {
  using namespace cs7gv5::ass2_2;

  figine::global::init();

  GLFWwindow *window = figine::global::win_mgr::create_window(
      800, 600, "cs7gv5 W/A/S/D/1/2 to move target, arrow keys to move camera",
      NULL, NULL);

  init();

  solver_t solver({
      {0.06f, 0.435f, -0.01f},
      {0.124663f, 0.377265f, -0.000762f},
      {0.188788f, 0.317575f, 0.005152f},
      {0.202000f, 0.275000f, 0.010000f},
  });

  target_obj_t joint1({0, 0, 0}, &camera);
  target_obj_t joint2({0, 0, 0}, &camera);
  target_obj_t joint3({0, 0, 0}, &camera);
  target_obj_t joint4({0, 0, 0}, &camera);

  joint1.init();
  joint2.init();
  joint3.init();
  joint4.init();

  target.pos = {0.18f, 0.31f, 0.01f};

  camera.lock({0, 0.3, 0});
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    process_input(window, 0.1);

    solver.solve(target.pos);
    auto res = solver.get_bone_transforms();

    joint1.pos = solver.joints[0];
    joint2.pos = solver.joints[1];
    joint3.pos = solver.joints[2];
    joint4.pos = solver.joints[3];

    animator->exec({
        {"L_UpperArm", res[0]},
        {"L_Forearm", res[1]},
        {"L_Hand", res[2]},
    });

    peter.loop(peter_shader);
    skybox.loop();

    joint1.loop();
    joint2.loop();
    joint3.loop();
    joint4.loop();

    target.loop();
    LOG_INFO("%s", glm::to_string(target.pos).c_str());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}
