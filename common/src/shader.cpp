#include <fstream>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <vector>

#include <boost/pfr.hpp>

#include <gl/gl.hpp>
#include <gl/shader.hpp>

// internal func declare
namespace {

common::result_t<std::shared_ptr<std::string>>
load_shader(const std::string &file);

common::result_t<GLuint> compile_shader(const std::string &glsl_file,
                                        const GLenum shader_type);

} // namespace

GLuint gl::shader_t::program_id() const { return _program_id; }

gl::shader_t::shader_t(const std::string &vert_glsl,
                       const std::string &frag_glsl,
                       std::shared_ptr<shader_profile_t> profile)
    : _vert_glsl(vert_glsl), _frag_glsl(frag_glsl), _profile(profile) {}

common::result_t<GLuint> gl::shader_t::build() {
  if (auto res = create(); res.err != std::nullopt) {
    LOG_ERR(res.err.value());
    return {0, res.err};
  }

  if (auto res = compile(); res.err != std::nullopt) {
    LOG_ERR(res.err.value());
    return {0, res.err};
  }

  if (auto res = link(); res.err != std::nullopt) {
    LOG_ERR(res.err.value());
    return {0, res.err};
  }

  return {_program_id, std::nullopt};
}

void gl::shader_t::use() { glUseProgram(_program_id); }

common::result_t<GLuint> gl::shader_t::create() {
  _program_id = glCreateProgram();
  if (_program_id == 0) {
    constexpr auto err = "error creating shader program";
    LOG_ERR(err);
    return {0, err};
  }

  return {_program_id, std::nullopt};
}

common::result_t<> gl::shader_t::compile() {
  auto res = compile_shader(_vert_glsl, GL_VERTEX_SHADER);
  if (res.err != std::nullopt) {
    LOG_ERR(res.err.value());
    return {common::none_v, res.err};
  }
  GLuint vert_id = res.result;

  res = compile_shader(_frag_glsl, GL_FRAGMENT_SHADER);
  if (res.err != std::nullopt) {
    LOG_ERR(res.err.value());
    return {common::none_v, res.err};
  }
  GLuint frag_id = res.result;

  glAttachShader(_program_id, vert_id);
  glAttachShader(_program_id, frag_id);

  glDeleteShader(vert_id);
  glDeleteShader(frag_id);

  return {common::none_v, std::nullopt};
}

common::result_t<> gl::shader_t::link() {
  glLinkProgram(_program_id);

  GLint success = 0;
  glGetProgramiv(_program_id, GL_LINK_STATUS, &success);
  if (!success) {
    GLchar err_log[1024] = {'\0'};
    glGetProgramInfoLog(_program_id, sizeof(err_log), NULL, err_log);
    auto err = common::make_str("Error linking shader program: ", err_log);
    LOG_ERR(err);
    return {common::none_v, err};
  }

  return {common::none_v, std::nullopt};
}

common::result_t<> gl::shader_t::validate() const {
  glValidateProgram(_program_id);

  GLint success = 0;
  GLchar err_log[1024] = {'\0'};
  glGetProgramiv(_program_id, GL_VALIDATE_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(_program_id, sizeof(err_log), NULL, err_log);
    auto err = common::make_str("Invalid shader program: ", err_log);
    LOG_ERR(err);
    return {common::none_v, err};
  }

  return {common::none_v, std::nullopt};
}

void gl::shader_t::set_profile() {
  if (_profile == nullptr) {
    return;
  }

  const meta_profile_t &meta = _profile->meta();
  boost::pfr::for_each_field(meta, [this](const auto &field) {
    for (const auto &pair : field) {
      set_uniform(pair.first, pair.second);
    }
  });
}

std::shared_ptr<gl::shader_profile_t> gl::shader_t::profile() {
  return _profile;
}

// internal func impl
namespace {

common::result_t<std::shared_ptr<std::string>>
load_shader(const std::string &file) {
  std::ifstream fs;

  fs.open(file);
  if (!fs.is_open()) {
    return {nullptr, common::make_str("fail to open file: ", file)};
  }
  defer(fs.close());

  std::stringstream ss;
  while (!fs.eof()) {
    std::string line;
    std::getline(fs, line);
    ss << line << "\n";
  }

  return {std::make_shared<std::string>(ss.str()), std::nullopt};
}

common::result_t<GLuint> compile_shader(const std::string &glsl_file,
                                        const GLenum shader_type) {
  auto res = load_shader(glsl_file);
  if (res.err != std::nullopt) {
    LOG_ERR(res.err.value());
    return {0, res.err};
  }

  GLuint shader_id = glCreateShader(shader_type);
  if (shader_id == 0) {
    const auto err = common::make_str(
        "filename: ", glsl_file, ", error creating shader type ", shader_type);
    LOG_ERR(err);
    return {0, err};
  }
  const GLchar *glsl_code = res.result->c_str();

  glShaderSource(shader_id, 1, (const GLchar **)&glsl_code, NULL);
  glCompileShader(shader_id);

  GLint success;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar log[1024];
    glGetShaderInfoLog(shader_id, 1024, NULL, log);
    const auto err = common::make_str("filename: ", glsl_file,
                                      ", error compiling shader type ",
                                      shader_type, ":", log);
    return {0, err};
  }

  return {shader_id, std::nullopt};
}

} // namespace
