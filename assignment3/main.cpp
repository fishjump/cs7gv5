#include "figine/figine.hpp"

#include "alien.h"
#include "mesh.h"
#include "target_obj.hpp"

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Sparse>
#include <glm/ext/matrix_projection.hpp>
#include <limits>

// ============= Global Variables =============

namespace cs7gv5::ass3 {

figine::core::camera_t camera({0.0f, 0.3f, 0.75f});

cs7gv5::ass3::console_t console;
target_obj_t target{{0, 0, 0}, &camera};
figine::core::shader_if alien_shader(alien_vs, alien_fs);
mesh_t alien = mesh_t::make("model/neutral.obj");
std::vector<mesh_t> faces = {
    mesh_t::make("model/neutral.obj"),
    mesh_t::make("model/Mery_jaw_open.obj"),
    mesh_t::make("model/Mery_kiss.obj"),
    mesh_t::make("model/Mery_l_brow_lower.obj"),
    mesh_t::make("model/Mery_l_brow_narrow.obj"),
    mesh_t::make("model/Mery_l_brow_raise.obj"),
    mesh_t::make("model/Mery_l_eye_closed.obj"),
    mesh_t::make("model/Mery_l_eye_lower_open.obj"),
    mesh_t::make("model/Mery_l_eye_upper_open.obj"),
    mesh_t::make("model/Mery_l_nose_wrinkle.obj"),
    mesh_t::make("model/Mery_l_puff.obj"),
    mesh_t::make("model/Mery_l_sad.obj"),
    mesh_t::make("model/Mery_l_smile.obj"),
    mesh_t::make("model/Mery_l_suck.obj"),
    mesh_t::make("model/Mery_r_brow_lower.obj"),
    mesh_t::make("model/Mery_r_brow_narrow.obj"),
    mesh_t::make("model/Mery_r_brow_raise.obj"),
    mesh_t::make("model/Mery_r_eye_closed.obj"),
    mesh_t::make("model/Mery_r_eye_lower_open.obj"),
    mesh_t::make("model/Mery_r_eye_upper_open.obj"),
    mesh_t::make("model/Mery_r_nose_wrinkle.obj"),
    mesh_t::make("model/Mery_r_puff.obj"),
    mesh_t::make("model/Mery_r_sad.obj"),
    mesh_t::make("model/Mery_r_smile.obj"),
    mesh_t::make("model/Mery_r_suck.obj"),
};

Eigen::VectorXf f0;
Eigen::MatrixXf B;
Eigen::VectorXf w;
Eigen::VectorXf f;

Eigen::VectorXf saved_w;
Eigen::VectorXf diff;
std::vector<size_t> cons_pos;
std::vector<glm::vec3> m_vec;
float animation_duration = 1.0f;
bool playing = false;

float current = 0.0f;
float last = 0.0f;
float delta = 0.0f;

} // namespace cs7gv5::ass3

// =========== End Global Variables ===========

void init() {
  using namespace cs7gv5::ass3;

  alien_shader.build();
  target.init();

  const size_t num_of_vertices = alien.vertices_sz;
  f0 = Eigen::VectorXf::Zero(num_of_vertices * 3, 1);
  f = Eigen::VectorXf::Zero(num_of_vertices * 3, 1);
  B = Eigen::MatrixXf::Zero(num_of_vertices * 3, faces.size());
  w = Eigen::VectorXf::Zero(faces.size(), 1);
  saved_w = Eigen::VectorXf::Zero(faces.size(), 1);
  diff = Eigen::VectorXf::Zero(faces.size(), 1);
  w(0) = 1;

  for (int i = 0; i < num_of_vertices * 3; i++) {
    f0(i) = faces[0].vertices[i];
  }
  f = f0;

  for (int i = 0; i < faces.size(); i++) {
    for (int j = 0; j < num_of_vertices * 3; j++) {
      B(j, i) = faces[i].vertices[j] - f0[j];
    }
  }
}

void render(const mesh_t &mesh) {
  using namespace cs7gv5::ass3;

  static uint32_t vao = 0, vbo_vert = 0, vbo_norm = 0;

  if (vao == 0) {
    glGenVertexArrays(1, &vao);
  }

  if (vbo_vert == 0) {
    glGenBuffers(1, &vbo_vert);
  }

  if (vbo_norm == 0) {
    glGenBuffers(1, &vbo_norm);
  }

  glBindVertexArray(vao);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_vert);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.vertices.size(),
               mesh.vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_norm);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.normals.size(),
               mesh.normals.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  figine::builtin::shader::light_t light = {
      .position = {0.0f, 2.0f, 8.0f},
      .diffuse_color = glm::vec3(0.5f),
      .ambient_color = glm::vec3(1.0f),
      .specular_color = glm::vec3(1.0f),
  };

  figine::builtin::shader::material_t material = {
      .shininess = 16,
      .ambient_color = glm::vec3(0.5f),
      .diffuse_color = glm::vec3(1.0f),
      .specular_color = glm::vec3(1.0f),
  };

  glm::mat4 model(1.0f);
  model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
  alien_shader.use();
  alien_shader.set_uniform("view_pos", camera.position);
  alien_shader.set_uniform(
      "projection",
      glm::perspective(glm::radians(camera.zoom),
                       figine::global::win_mgr::aspect_ratio(), 0.1f, 100.0f));
  alien_shader.set_uniform("transform", model);
  alien_shader.set_uniform("view", camera.view_matrix());

  alien_shader.set_uniform("light.position", light.position);
  alien_shader.set_uniform("light.ambient_color", light.ambient_color);
  alien_shader.set_uniform("light.diffuse_color", light.diffuse_color);
  alien_shader.set_uniform("light.specular_color", light.specular_color);

  alien_shader.set_uniform("material.shininess", material.shininess);
  alien_shader.set_uniform("material.ambient_color", material.ambient_color);
  alien_shader.set_uniform("material.diffuse_color", material.diffuse_color);
  alien_shader.set_uniform("material.specular_color", material.specular_color);

  glDrawArrays(GL_TRIANGLES, 0, mesh.vertices_sz);
}

void process_input(GLFWwindow *window, float delta_time) {
  using namespace cs7gv5::ass3;

  delta_time *= 0.1f;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.position -= glm::vec3{0, 0, 1} * delta_time;
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.position += glm::vec3{0, 0, 1} * delta_time;
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.position -= glm::vec3{1, 0, 0} * delta_time;
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.position += glm::vec3{1, 0, 0} * delta_time;
  }

  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    camera.position -= glm::vec3{0, 1, 0} * delta_time;
  }

  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
    camera.position += glm::vec3{0, 1, 0} * delta_time;
  }

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    target.pos -= glm::vec3{0, 1, 0} * delta_time;
  }

  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    target.pos += glm::vec3{0, 1, 0} * delta_time;
  }

  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    target.pos -= glm::vec3{1, 0, 0} * delta_time;
  }

  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    target.pos += glm::vec3{1, 0, 0} * delta_time;
  }
}

int main(int argc, char **argv) {
  using namespace cs7gv5::ass3;

  figine::global::init();

  GLFWwindow *window =
      figine::global::win_mgr::create_window(800, 600, "cs7gv5", NULL, NULL);
  glfwSetCursorPosCallback(
      window, [](GLFWwindow *window, double x_pos_in, double y_pos_in) -> void {
        static int status = GLFW_RELEASE;
        static float _z = 0;
        int new_status = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

        double _x = 0, _y = 0;
        glfwGetCursorPos(window, &_x, &_y);

        float x = _x, y = figine::global::win_mgr::height - _y, z = 0;
        glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);

        auto proj = glm::perspective(glm::radians(camera.zoom),
                                     figine::global::win_mgr::aspect_ratio(),
                                     0.1f, 100.0f);
        glm::mat4 model(1.0f);
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
        glm::vec3 vertex_pos = glm::unProject(
            glm::vec3{x, y, z}, camera.view_matrix() * model, proj,
            glm::vec4{0.0f, 0.0f, figine::global::win_mgr::width,
                      figine::global::win_mgr::height});
        float min = 99999999;
        float min_idx = 0;
        for (size_t i = 0; i < alien.vertices.size(); i += 3) {
          glm::vec3 pos{alien.vertices[i], alien.vertices[i + 1],
                        alien.vertices[i + 2]};
          float tmp = glm::distance(vertex_pos, pos);
          if (tmp < min) {
            min = tmp;
            min_idx = i;
          }
        }

        if (new_status == GLFW_PRESS && status == GLFW_RELEASE) {
          LOG_INFO("begin");
          cons_pos.push_back(min_idx);
          m_vec.push_back({0, 0, 0});
          _z = z;
          LOG_INFO("begin at: %f %f %f", vertex_pos.x, vertex_pos.y,
                   vertex_pos.z);
        }

        if (new_status == GLFW_PRESS) {
          vertex_pos = glm::unProject(
              glm::vec3{x, y, _z}, camera.view_matrix() * model, proj,
              glm::vec4{0.0f, 0.0f, figine::global::win_mgr::width,
                        figine::global::win_mgr::height});
          m_vec[m_vec.size() - 1] = vertex_pos;

          float alpha = 0.1;
          float mu = 0.01;
          Eigen::MatrixXf B_(3 * cons_pos.size(), faces.size());
          for (size_t i = 0; i < faces.size(); i++) {
            for (size_t j = 0; j < cons_pos.size(); j++) {
              B_(3 * j, i) = faces[i].vertices[cons_pos[j]];
              B_(3 * j + 1, i) = faces[i].vertices[cons_pos[j] + 1];
              B_(3 * j + 2, i) = faces[i].vertices[cons_pos[j] + 2];
            }
          }

          Eigen::VectorXf m0(3 * cons_pos.size(), 1);
          Eigen::VectorXf m(3 * cons_pos.size(), 1);
          for (size_t i = 0; i < cons_pos.size(); i++) {
            m0(3 * i) = alien.vertices[cons_pos[i]];
            m0(3 * i + 1) = alien.vertices[cons_pos[i] + 1];
            m0(3 * i + 2) = alien.vertices[cons_pos[i] + 2];
          }

          for (size_t i = 0; i < m_vec.size(); i++) {
            m(3 * i) = m_vec[i].x;
            m(3 * i + 1) = m_vec[i].y;
            m(3 * i + 2) = m_vec[i].z;
          }

          Eigen::VectorXf wt = w;

          Eigen::MatrixXf lhs = B_.transpose() * B_ +
                                (alpha + mu) * Eigen::MatrixXf::Identity(
                                                   faces.size(), faces.size());
          Eigen::VectorXf rhs = B_.transpose() * (m - m0) + alpha * wt;

          Eigen::LDLT<Eigen::MatrixXf> solver(lhs);
          w = solver.solve(rhs);
        }

        auto n = 0.01f * glm::vec3{alien.vertices[min_idx],
                                   alien.vertices[min_idx + 1],
                                   alien.vertices[min_idx + 2]};
        target.pos = n;

        status = new_status;
      });
  init();

  figine::imnotgui::init(window);
  figine::imnotgui::register_window(&console);

  while (!glfwWindowShouldClose(window)) {
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    current = glfwGetTime();
    delta = current - last;
    last = current;

    process_input(window, delta);

    f = f0 + (B * w);

    for (size_t i = 0; i < alien.vertices.size(); i++) {
      alien.vertices[i] = f[i];
    }

    render(alien);
    target.loop();

    figine::imnotgui::render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}
