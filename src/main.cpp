#include "config.hpp"
#include "figine/figine.hpp"
#include "resource_index.hpp"

int main(int argc, char **argv) {
  figine::global::init();
  defer(glfwTerminate());

  GLFWwindow *window = figine::global::win_mgr::create_window(
      cs7gv5::SCR_WIDTH, cs7gv5::SCR_HEIGHT, "cs7gv5", NULL, NULL);

  figine::imnotgui::init(window);
  figine::imnotgui::register_window(&cs7gv5::global::console);

  cs7gv5::global::init();

  cs7gv5::global::camera_3rd.lock({0, 0, 0});

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cs7gv5::process_input(window);

    cs7gv5::global::camera_1st.position =
        figine::core::object_if::rotate(
            glm::vec3{glm::radians(cs7gv5::global::console.roll),
                      glm::radians(cs7gv5::global::console.pitch),
                      glm::radians(cs7gv5::global::console.yaw)},
            true, glm::mat4(1)) *
        glm::vec4{0, 3, 0, 0};

    cs7gv5::global::camera_1st.front =
        figine::core::object_if::rotate(
            glm::vec3{glm::radians(cs7gv5::global::console.roll),
                      glm::radians(cs7gv5::global::console.pitch),
                      glm::radians(cs7gv5::global::console.yaw)},
            true, glm::mat4(1)) *
        glm::vec4{0, 0, 1, 0};

    glViewport(0, 0, cs7gv5::SCR_WIDTH, cs7gv5::SCR_HEIGHT);
    if (cs7gv5::global::console.third_person_view) {
      cs7gv5::global::airplane.camera = &cs7gv5::global::camera_3rd;
      cs7gv5::global::skybox.camera = &cs7gv5::global::camera_3rd;

      cs7gv5::global::airplane.loop();
    } else {
      cs7gv5::global::airplane.camera = &cs7gv5::global::camera_1st;
      cs7gv5::global::skybox.camera = &cs7gv5::global::camera_1st;
    }
    cs7gv5::global::skybox.loop();

    glViewport(0, 0, cs7gv5::SCR_WIDTH * 0.2, cs7gv5::SCR_HEIGHT * 0.2);
    if (cs7gv5::global::console.third_person_view) {
      cs7gv5::global::airplane.camera = &cs7gv5::global::camera_1st;
      cs7gv5::global::skybox.camera = &cs7gv5::global::camera_1st;
    } else {
      cs7gv5::global::airplane.camera = &cs7gv5::global::camera_3rd;
      cs7gv5::global::skybox.camera = &cs7gv5::global::camera_3rd;

      cs7gv5::global::airplane.loop();
    }
    cs7gv5::global::skybox.loop();

    figine::imnotgui::render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}
