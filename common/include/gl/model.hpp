#ifndef GL_MODEL_HPP
#define GL_MODEL_HPP

#include <unordered_map>
#include <vector>

#include <assimp/scene.h>

#include "camera.hpp"
#include "mesh.hpp"

namespace gl {

class model_t {
public:
  model_t(const std::string &path, const gl::shader_t &shader,
          const gl::camera_t *camera, bool gamma_correction = false);

  const glm::mat4 &transform_mat() const;
  gl::shader_t &shader();
  std::shared_ptr<gl::shader_profile_t> profile();

  virtual void init();
  virtual void update();
  void loop();
  void bind_camera(const gl::camera_t *camera) { _camera = camera; }

  model_t &translate(const glm::vec3 &v);
  model_t &scale(const glm::vec3 &v);
  model_t &rotate(float degree, const glm::vec3 &axis);

private:
  bool _gamma_correction;

  const gl::camera_t *_camera;
  gl::shader_t _shader;
  glm::mat4 _transform_mat;

  std::string _dir;
  std::vector<mesh_t> _meshes;
  std::unordered_map<std::string, texture_t> _textures_cache;

  void draw();
  void update_profile();

  common::result_t<> load(const std::string &path);
  void process_node(aiNode *node, const aiScene *scene);
  mesh_t process_mesh(aiMesh *mesh, const aiScene *scene);
  std::vector<gl::texture_t>
  load_material_textures(aiMaterial *mat, aiTextureType type,
                         const std::string &typeName);
};

} // namespace gl

#endif // GL_MODEL_HPP