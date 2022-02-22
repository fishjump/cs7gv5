#include "config.hpp"
#include "figine/figine.hpp"
#include "resource.hpp"

int main(int argc, char **argv) {
  namespace gb = cs7gv5::global;

  figine::global::init();
  defer(glfwTerminate());

  GLFWwindow *window = figine::global::win_mgr::create_window(
      cs7gv5::SCR_WIDTH, cs7gv5::SCR_HEIGHT, "cs7gv5", NULL, NULL);

  figine::imnotgui::init(window);
  figine::imnotgui::register_window(&gb::console);

  gb::init();

  gb::camera_3rd.lock({0, 0, 0});
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cs7gv5::process_input(window);

    auto trans = figine::core::object_if::rotate(
        glm::vec3{glm::radians(gb::console.roll),
                  glm::radians(gb::console.pitch),
                  glm::radians(gb::console.yaw)},
        true, glm::mat4(1));

    gb::camera_1st.position = trans * glm::vec4{0, 0.2, 0, 0};
    gb::camera_1st.front = trans * glm::vec4{0, 0, -1, 0};

    glViewport(0, 0, figine::global::win_mgr::width,
               figine::global::win_mgr::height);
    if (gb::console.third_person_view) {
      gb::airplane.camera = &gb::camera_3rd;
      gb::skybox.camera = &gb::camera_3rd;

      gb::airplane.loop(gb::phong_shader);
    } else {
      gb::airplane.camera = &gb::camera_1st;
      gb::skybox.camera = &gb::camera_1st;
    }
    gb::skybox.loop();

    glViewport(0, 0, figine::global::win_mgr::width * 0.2,
               figine::global::win_mgr::height * 0.2);
    if (gb::console.third_person_view) {
      gb::airplane.camera = &gb::camera_1st;
      gb::skybox.camera = &gb::camera_1st;
    } else {
      gb::airplane.camera = &gb::camera_3rd;
      gb::skybox.camera = &gb::camera_3rd;

      gb::airplane.loop(gb::phong_shader);
    }
    gb::skybox.loop();

    figine::imnotgui::render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}
