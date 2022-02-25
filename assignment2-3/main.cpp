#include "FABRIK_solver.hpp"
#include "figine/figine.hpp"
#include "resource.hpp"

void process_input(GLFWwindow *window, float delta_time) {
  using namespace cs7gv5::ass2_3;

  delta_time *= 0.1f;

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
  using namespace cs7gv5::ass2_3;

  figine::global::init();

  GLFWwindow *window = figine::global::win_mgr::create_window(
      800, 600, "cs7gv5 click do anime to run animation", NULL, NULL);

  init(window);

  target_obj_t joint1({0, 0, 0}, &camera);
  target_obj_t joint2({0, 0, 0}, &camera);
  target_obj_t joint3({0, 0, 0}, &camera);

  joint1.init();
  joint2.init();
  joint3.init();

  camera.lock({0, 0.3, 0});
  float last = 0;
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float current = glfwGetTime();
    delta_time = current - last;
    last = current;
    process_input(window, 0.1);

    solver.solve(target.pos);
    auto res = solver.get_bone_transforms();

    joint1.pos = solver.joints[0];
    joint2.pos = solver.joints[1];
    joint3.pos = solver.joints[2];

    animator->exec({
        {"L_UpperArm", res[0]},
        {"L_Forearm", res[1]},
    });

    peter.loop(peter_shader);
    skybox.loop();

    joint1.loop();
    joint2.loop();
    joint3.loop();

    target.loop();

    figine::imnotgui::render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}
