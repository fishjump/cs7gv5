
#include <ft2build.h>
#include FT_FREETYPE_H

#include <common.hpp>
#include <gl/freetype_gl.hpp>
#include <gl/gl.hpp>

namespace {

GLuint VAO = 0, VBO = 0;

}

void gl::freetype_gl::init(shader_t &shader, const size_t win_width,
                           const size_t win_height) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(win_width), 0.0f,
                                    static_cast<float>(win_height));
  shader.use();
  glUniformMatrix4fv(glGetUniformLocation(shader.program_id(), "projection"), 1,
                     GL_FALSE, glm::value_ptr(projection));
}

std::shared_ptr<gl::freetype_gl::font_t>
gl::freetype_gl::load_font(const std::string &font_path) {
  FT_Library ft;
  if (FT_Init_FreeType(&ft)) {
    LOG_ERR("Could not init FreeType Library");
    return nullptr;
  }
  defer(FT_Done_FreeType(ft));

  FT_Face face;
  if (FT_New_Face(ft, font_path.c_str(), 0, &face)) {
    LOG_ERR("Failed to load font");
    return nullptr;
  }
  defer(FT_Done_Face(face));

  FT_Set_Pixel_Sizes(face, 0, 48);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  auto font_ptr = std::make_shared<font_t>();
  for (uint8_t c = 0; c < 128; c++) {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      LOG_ERR("Failed to load Glyph");
      continue;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                 face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                 face->glyph->bitmap.buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    character_t character = {
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        static_cast<unsigned int>(face->glyph->advance.x)};
    font_ptr->insert(std::pair<char, character_t>(c, character));
  }
  glBindTexture(GL_TEXTURE_2D, 0);

  return font_ptr;
}

void gl::freetype_gl::print(
    const std::shared_ptr<gl::freetype_gl::font_t> font_ptr, shader_t &shader,
    const std::string &str, float x, float y, float scale,
    const glm::vec3 &color) {

  shader.use();
  glUniform3fv(glGetUniformLocation(shader.program_id(), "text_color"), 1,
               &color[0]);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(VAO);

  defer(glBindVertexArray(0));
  defer(glBindTexture(GL_TEXTURE_2D, 0));

  for (const auto &ch : str) {
    character_t character_v = (*font_ptr)[ch];

    float x_pos = x + character_v.bearing.x * scale;
    float y_pos = y - (character_v.size.y - character_v.bearing.y) * scale;

    float w = character_v.size.x * scale;
    float h = character_v.size.y * scale;

    float vertices[6][4] = {
        {x_pos, y_pos + h, 0.0f, 0.0f},    {x_pos, y_pos, 0.0f, 1.0f},
        {x_pos + w, y_pos, 1.0f, 1.0f},

        {x_pos, y_pos + h, 0.0f, 0.0f},    {x_pos + w, y_pos, 1.0f, 1.0f},
        {x_pos + w, y_pos + h, 1.0f, 0.0f}};

    glBindTexture(GL_TEXTURE_2D, character_v.texture_id);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    x += (character_v.advance >> 6) * scale;
  }
}
