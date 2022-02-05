#ifndef GL_MESH_HPP
#define GL_MESH_HPP

#include <array>
#include <memory>
#include <vector>

#include <boost/pfr.hpp>

#include <common.hpp>

#include "gl.hpp"
#include "shader.hpp"

namespace gl {

constexpr size_t MAX_BONE_INFLUENCE = 4;

struct vertex_t {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texture_coordinate;
  glm::vec3 tangent;
  glm::vec3 bitangent;
  std::array<int, MAX_BONE_INFLUENCE> bone_ids;
  std::array<float, MAX_BONE_INFLUENCE> bone_weights;
};

struct texture_t {
  uint32_t id;
  std::string type;
  std::string path;
};

class mesh_t final {
public:
  mesh_t(const std::vector<vertex_t> &vertices,
         const std::vector<uint32_t> &indices,
         const std::vector<texture_t> &textures);

  void draw(const shader_t &shader) const;

private:
  GLuint _vao = 0;
  GLuint _vbo = 0;
  GLuint _ebo = 0;

  std::vector<vertex_t> _vertices;
  std::vector<uint32_t> _indices;
  std::vector<texture_t> _textures;

  void init();
};

} // namespace gl

#endif // GL_MESH_HPP