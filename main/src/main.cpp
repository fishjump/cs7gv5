#include <array>
#include <iostream>
#include <memory>
#include <optional>
#include <vector>

#include <airplane_model.hpp>
#include <common.hpp>
#include <gl.hpp>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>
#include <imnotgui/imnotgui.hpp>
#include <io.hpp>
#include <program_options.hpp>

namespace {

gl::light_t light = {
    .position = {0.0f, 2.0f, 8.0f},
    .diffuse_color = gl::white * glm::vec3(0.5f),
    .ambient_color = gl::white,
    .specular_color = glm::vec3(1.0f),
};

gl::material_t material = {.shininess = 16,
                           .ambient_color = gl::gray,
                           .diffuse_color = gl::gray,
                           .specular_color = gl::gray};

float last_frame = 0.0f;

} // namespace

int main(int argc, char **argv) {
  gl::init();
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

  imnotgui::init(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    LOG_ERR("Failed to initialize GLAD");
    return -1;
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // create shaders
  gl::shader_t phong_shader(
      "shader/base.vs", "shader/phong.fs",
      std::make_shared<gl::phong_profile_t>(material, light));
  gl::shader_t gooch_shader(
      "shader/base.vs", "shader/gooch.fs",
      std::make_shared<gl::gooch_profile_t>(material, light));
  gl::shader_t font_shader("shader/font.vs", "shader/font.fs");

  phong_shader.build();
  gooch_shader.build();
  font_shader.build();

  gl::freetype_gl::init(font_shader, cs7gv5::SCR_WIDTH, cs7gv5::SCR_HEIGHT);
  auto font = gl::freetype_gl::load_font("/Library/Fonts/Arial Unicode.ttf");

  cs7gv5::airplane_t airplane({0, 0, 0}, phong_shader, &cs7gv5::camera());

  ImGui::StyleColorsDark();
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  imnotgui::demo_window_t demo_win;
  imnotgui::register_window(&demo_win);

  airplane.init();

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float current_frame = (float)glfwGetTime();
    cs7gv5::delta_time() = current_frame - last_frame;
    last_frame = current_frame;

    cs7gv5::process_input(window);

    gl::freetype_gl::print(font, font_shader, "This is sample text", 25.0f,
                           25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
    gl::freetype_gl::print(font, font_shader, "(C) LearnOpenGL.com", 540.0f,
                           570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

    airplane.loop();

    imnotgui::render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}
