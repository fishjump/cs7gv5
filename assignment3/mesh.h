#pragma once

#include "figine/figine.hpp"

#include <cstdio>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>

// ============ Declearations ============

struct mesh_t;

namespace _impl {

inline void load(Assimp::Importer &importer, std::vector<aiMesh *> &ai_mesh,
                 const std::string &path);

} // namespace _impl

// ========== End Declearations ===========

struct mesh_t {
  std::vector<float> vertices;
  std::vector<float> normals;
  std::vector<float> tex_coords;
  size_t vertices_sz;

  inline static mesh_t make(const std::string &path) {
    Assimp::Importer importer;

    std::vector<aiMesh *> ai_mesh;
    _impl::load(importer, ai_mesh, path);

    mesh_t mesh;
    for (const auto p : ai_mesh) {
      mesh.vertices_sz += p->mNumVertices;
      for (size_t i = 0; i < p->mNumVertices; i++) {
        mesh.vertices.push_back(p->mVertices[i].x);
        mesh.vertices.push_back(p->mVertices[i].y);
        mesh.vertices.push_back(p->mVertices[i].z);

        assert(p->HasNormals());
        mesh.normals.push_back(p->mNormals[i].x);
        mesh.normals.push_back(p->mNormals[i].y);
        mesh.normals.push_back(p->mNormals[i].z);

        assert(p->mTextureCoords[0]);
        mesh.tex_coords.push_back(p->mTextureCoords[0][i].x);
        mesh.tex_coords.push_back(p->mTextureCoords[0][i].y);
      }
    }

    return mesh;
  }
};

// ============ Internal Impelment ============

namespace _impl {

inline void find_available_mesh(std::vector<aiMesh *> &ai_mesh,
                                const aiNode *node, const aiScene *scene) {
  for (size_t i = 0; i < node->mNumMeshes; i++) {
    ai_mesh.push_back(scene->mMeshes[node->mMeshes[i]]);
  }

  for (size_t i = 0; i < node->mNumChildren; i++) {
    find_available_mesh(ai_mesh, node->mChildren[i], scene);
  }
}

inline void load(Assimp::Importer &importer, std::vector<aiMesh *> &ai_mesh,
                 const std::string &path) {
  const aiScene *scene = importer.ReadFile(
      path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
  if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) ||
      !scene->mRootNode) {
    LOG_ERR("%s", importer.GetErrorString());
  }

  find_available_mesh(ai_mesh, scene->mRootNode, scene);
}

} // namespace _impl

// ============ End Impelment ============