#include <gl.hpp>

gl::mesh_t::mesh_t(const std::vector<vertex_t> &vertices,
                   const std::vector<uint32_t> &indices,
                   const std::vector<texture_t> &textures)
    : _vertices(vertices), _indices(indices), _textures(textures) {
  init();
}

void gl::mesh_t::init() {
  glGenVertexArrays(1, &_vao);
  glGenBuffers(1, &_vbo);
  glGenBuffers(1, &_ebo);

  glBindVertexArray(_vao);
  defer(glBindVertexArray(NULL));

  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glBufferData(GL_ARRAY_BUFFER,
               _vertices.size() *
                   sizeof(common::value_type_t<decltype(_vertices)>),
               _vertices.cbegin().base(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               _indices.size() *
                   sizeof(common::value_type_t<decltype(_indices)>),
               _indices.cbegin().base(), GL_STATIC_DRAW);

  const auto b = *_vertices.cbegin();
  boost::pfr::for_each_field(b, [&b](const auto &field, int index) {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, sizeof(field) / sizeof(GL_FLOAT), GL_FLOAT,
                          GL_FALSE, sizeof(b),
                          (void *)common::offset_diff(field, b));
  });
}

void gl::mesh_t::draw(const shader_t &shader) const {
  uint32_t diffuseNr = 1, specularNr = 1, normalNr = 1, heightNr = 1;
  for (size_t i = 0; i < _textures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    std::string number = "", name = _textures[i].type;
    if (name == "texture_diffuse") {
      number = std::to_string(diffuseNr++);
    } else if (name == "texture_specular") {
      number = std::to_string(specularNr++);
    } else if (name == "texture_normal") {
      number = std::to_string(normalNr++);
    } else if (name == "texture_height") {
      number = std::to_string(heightNr++);
    }

    glUniform1i(
        glGetUniformLocation(shader.program_id(), (name + number).c_str()), i);
    glBindTexture(GL_TEXTURE_2D, _textures[i].id);
  }

  glBindVertexArray(_vao);
  defer(glBindVertexArray(NULL));

  glDrawElements(GL_TRIANGLES, (uint32_t)_indices.size(), GL_UNSIGNED_INT, 0);
}