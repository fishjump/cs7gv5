#ifndef COMMON_IMNOTGUI_IMNOTGUI_HPP
#define COMMON_IMNOTGUI_IMNOTGUI_HPP

#include <GLFW/glfw3.h>

namespace imnotgui {

class window_t {
public:
  virtual void refresh() = 0;
};

class demo_window_t : public window_t {
private:
  bool show_demo_window;
  bool show_another_window;
  bool clear_color;

public:
  void refresh();
};

void init(GLFWwindow *window);
void render();
void register_window(window_t *window);

} // namespace imnotgui

#endif // COMMON_IMNOTGUI_IMNOTGUI_HPP