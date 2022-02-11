#include <cs7gvx_utils.hpp>

#include <io.hpp>
#include <resource_index.hpp>

int main(int argc, char **argv) {
  cs7gvx_utils::gl::init();
  defer(glfwTerminate());

  GLFWwindow *window = glfwCreateWindow(cs7gv5::SCR_WIDTH, cs7gv5::SCR_HEIGHT,
                                        "cs7gv5", NULL, NULL);
  if (window == NULL) {
    LOG_ERR("Failed to create GLFW window");
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, cs7gv5::framebuffer_size_callback);
  // glfwSetCursorPosCallback(window, cs7gv5::mouse_callback);
  // glfwSetScrollCallback(window, cs7gv5::scroll_callback);
  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    LOG_ERR("Failed to initialize GLAD");
    return -1;
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  cs7gvx_utils::imnotgui::init(window);
  cs7gvx_utils::imnotgui::register_window(&cs7gv5::global::console);

  cs7gv5::global::init();

  cs7gv5::global::camera_3rd.lock({0, 0, 0});

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cs7gv5::process_input(window);

    cs7gv5::global::camera_1st.position =
        cs7gvx_utils::gl::model_t::rotate(
            glm::vec3{glm::radians(cs7gv5::global::console._roll),
                      glm::radians(cs7gv5::global::console._pitch),
                      glm::radians(cs7gv5::global::console._yaw)},
            true, glm::mat4(1)) *
        glm::vec4{0, 3, 0, 0};

    cs7gv5::global::camera_1st.front =
        cs7gvx_utils::gl::model_t::rotate(
            glm::vec3{glm::radians(cs7gv5::global::console._roll),
                      glm::radians(cs7gv5::global::console._pitch),
                      glm::radians(cs7gv5::global::console._yaw)},
            true, glm::mat4(1)) *
        glm::vec4{0, 0, 1, 0};

    glViewport(0, 0, cs7gv5::SCR_WIDTH, cs7gv5::SCR_HEIGHT);
    if (cs7gv5::global::console._3rd_person_view) {
      cs7gv5::global::airplane.camera = &cs7gv5::global::camera_3rd;
      cs7gv5::global::skybox.camera = &cs7gv5::global::camera_3rd;

      cs7gv5::global::airplane.loop();
    } else {
      cs7gv5::global::airplane.camera = &cs7gv5::global::camera_1st;
      cs7gv5::global::skybox.camera = &cs7gv5::global::camera_1st;
    }
    cs7gv5::global::skybox.loop();

    glViewport(0, 0, cs7gv5::SCR_WIDTH * 0.2, cs7gv5::SCR_HEIGHT * 0.2);
    if (cs7gv5::global::console._3rd_person_view) {
      cs7gv5::global::airplane.camera = &cs7gv5::global::camera_1st;
      cs7gv5::global::skybox.camera = &cs7gv5::global::camera_1st;
    } else {
      cs7gv5::global::airplane.camera = &cs7gv5::global::camera_3rd;
      cs7gv5::global::skybox.camera = &cs7gv5::global::camera_3rd;

      cs7gv5::global::airplane.loop();
    }
    cs7gv5::global::skybox.loop();

    cs7gvx_utils::imnotgui::render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}
