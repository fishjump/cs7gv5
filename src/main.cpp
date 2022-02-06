#include <cs7gvx_utils.hpp>

#include <global.hpp>
#include <io.hpp>

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
  // glfwSetFramebufferSizeCallback(window, cs7gv5::framebuffer_size_callback);
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

  cs7gv5::global::phong_shader.build();
  cs7gv5::global::font_shader.build();
  cs7gv5::global::airplane.init();

  cs7gvx_utils::freetype_gl::init(cs7gv5::global::font_shader,
                                  cs7gv5::SCR_WIDTH, cs7gv5::SCR_HEIGHT);
  auto font =
      cs7gvx_utils::freetype_gl::load_font("/Library/Fonts/Arial Unicode.ttf");

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, cs7gv5::SCR_WIDTH * 0.2, cs7gv5::SCR_HEIGHT * 0.2);
    cs7gv5::global::airplane.loop();

    glViewport(0, 0, cs7gv5::SCR_WIDTH, cs7gv5::SCR_HEIGHT);
    cs7gv5::global::airplane.loop();

    cs7gv5::process_input(window);

    // cs7gvx_utils::freetype_gl::print(font, cs7gv5::global::font_shader,
    //                                  "This is sample text ", 25.0f, 25.0f, 1.0f,
    //                                  glm::vec3(0.5, 0.8f, 0.2f));
    // cs7gvx_utils::freetype_gl::print(font, cs7gv5::global::font_shader,
    //                                  "(C) LearnOpenGL.com", 540.0f, 570.0f,
    //                                  0.5f, glm::vec3(0.3, 0.7f, 0.9f));

    cs7gvx_utils::imnotgui::render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}
