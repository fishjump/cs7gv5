#include <imnotgui/imnotgui.hpp>

#include <memory>
#include <unordered_set>

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

namespace {

std::unordered_set<imnotgui::window_t *> windows;

}

void imnotgui::init(GLFWwindow *window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();
}

void imnotgui::render() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  for (window_t *win : windows) {
    win->refresh();
  }

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void imnotgui::register_window(imnotgui::window_t *window) {
  windows.insert(window);
}