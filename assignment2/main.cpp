#include "config.hpp"
#include "figine/figine.hpp"
#include "resource.hpp"

int main(int argc, char **argv) {
  namespace gb = cs7gv5::global;

  figine::global::init();
  defer(glfwTerminate());

  GLFWwindow *window = figine::global::win_mgr::create_window(
      cs7gv5::SCR_WIDTH, cs7gv5::SCR_HEIGHT, "cs7gv5", NULL, NULL);

  cs7gv5::global::init();

  cs7gv5::global::camera.lock({0, 0.3, 0});
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cs7gv5::process_input(window);

    cs7gv5::global::peter.loop(gb::peter_shader);
    cs7gv5::global::skybox.loop();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}
