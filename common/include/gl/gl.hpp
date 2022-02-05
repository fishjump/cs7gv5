#ifndef GL_GL_HPP
#define GL_GL_HPP

#ifdef __APPLE__

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#elif __linux__

#include <GL/freeglut.h>
#include <GL/glew.h>

#else

#endif

namespace gl {

void init();

inline constexpr glm::vec3 white(1.0f);
inline constexpr glm::vec3 black(0.0f);
inline constexpr glm::vec3 gray(0.5f);
inline constexpr glm::vec3 red = {1.0f, 0.0f, 0.0f};
inline constexpr glm::vec3 green = {0.0f, 1.0f, 0.0f};
inline constexpr glm::vec3 blue = {0.0f, 0.0f, 1.0f};

struct material_t {
  GLfloat shininess;
  glm::vec3 ambient_color;
  glm::vec3 diffuse_color;
  glm::vec3 specular_color;
};

struct light_t {
  glm::vec3 position;
  glm::vec3 ambient_color;
  glm::vec3 diffuse_color;
  glm::vec3 specular_color;
};

} // namespace gl

#endif // GL_GL_HPP