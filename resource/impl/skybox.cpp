#include "stb_image.h"
#include <resource_index.hpp>
#include <skybox.hpp>

cs7gv5::models::skybox_t::skybox_t(const std::array<std::string, 6> &faces)
    : _faces(faces) {
  this->shader = &cs7gv5::global::skybox_shader;
  this->camera = &cs7gv5::global::camera_3rd;
  transform = scale(glm::vec3(10.0f));
}

void cs7gv5::models::skybox_t::init() {
  static float vertices[] = {
      -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
      1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

      -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
      -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

      1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

      -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

      -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

      -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
      1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

  // model
  GLuint vbo = 0;
  glGenVertexArrays(1, &_vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(_vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

  // texture
  glGenTextures(1, &_box_texture);
  glBindTexture(GL_TEXTURE_CUBE_MAP, _box_texture);

  int width, height, nr_components;
  for (size_t i = 0; i < _faces.size(); i++) {
    uint8_t *data =
        stbi_load(_faces[i].c_str(), &width, &height, &nr_components, 0);
    defer(stbi_image_free(data));
    if (data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height,
                   0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else {
      LOG_ERR("cubemap texture failed to load at path: ", _faces[i]);
    }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void cs7gv5::models::skybox_t::loop() {
  // update
  this->shader->profile->projection = glm::perspective(
      glm::radians(camera->zoom), (float)800 / (float)600, 0.1f, 100.0f);
  this->shader->profile->view = camera->view_matrix();
  this->shader->profile->model = transform;

  glDepthFunc(GL_LEQUAL);
  shader->use();
  shader->set_profile();

  glBindVertexArray(_vao);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, _box_texture);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthFunc(GL_LESS);
}

// right, left, top, bottom, front, back
cs7gv5::models::skybox_t
    cs7gv5::global::skybox({"model/skybox/right.jpg", "model/skybox/left.jpg",
                            "model/skybox/top.jpg", "model/skybox/bottom.jpg",
                            "model/skybox/front.jpg", "model/skybox/back.jpg"});
