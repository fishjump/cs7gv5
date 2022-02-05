#ifndef COMMON_GL_FREETYPE_GL_HPP
#define COMMON_GL_FREETYPE_GL_HPP

#include <map>
#include <memory>

#include "gl.hpp"
#include "shader.hpp"

namespace gl::freetype_gl {

struct character_t {
  GLuint texture_id;  // ID handle of the glyph texture
  glm::ivec2 size;    // Size of glyph
  glm::ivec2 bearing; // Offset from baseline to left/top of glyph
  GLuint advance;     // Offset to advance to next glyph
};

using font_t = std::map<GLchar, character_t>;

void init(shader_t &shader, const size_t win_width, const size_t win_height);

std::shared_ptr<font_t> load_font(const std::string &path);
void print(const std::shared_ptr<gl::freetype_gl::font_t> font_ptr,
           shader_t &shader, const std::string &str, float x, float y,
           float scale, const glm::vec3 &color);

} // namespace gl::freetype_gl

#endif // COMMON_GL_FREETYPE_GL_HPP