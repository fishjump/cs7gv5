#ifndef COMMON_COMMON_HPP
#define COMMON_COMMON_HPP

#include <optional>
#include <sstream>
#include <stack>
#include <string>
#include <type_traits>

#include <gl/gl.hpp>

#include "log.hpp"

#define __MERGE(x, y) x_##y
#define _MERGE(x, y) __MERGE(x, y)
#define UNIQUE(x) _MERGE(x, __COUNTER__)

#define defer(expr) common::_defer_t UNIQUE(_defer) = [&]() { expr; }

namespace common {

template <class> inline constexpr bool dependent_false_v = false;

template <class T, int _sz> struct is_glm_vec : public std::false_type {};
template <class T, int _sz, glm::qualifier Q>
struct is_glm_vec<glm::vec<_sz, T, Q>, _sz> : public std::true_type {};
template <class T, int _sz>
inline constexpr bool is_glm_vec_v = is_glm_vec<T, _sz>::value;

template <class T, int _sz> struct is_glm_mat : public std::false_type {};
template <class T, int _sz, glm::qualifier Q>
struct is_glm_mat<glm::mat<_sz, _sz, T, Q>, _sz> : public std::true_type {};
template <class T, int _sz>
inline constexpr bool is_glm_mat_v = is_glm_mat<T, _sz>::value;

struct none_t {};
constexpr none_t none_v;

template <class T = none_t> struct result_t {
  T result;
  std::optional<std::string> err;
};

template <class T, class U> inline size_t offset_diff(const T &t, const U &u) {
  return ((size_t)(&t) - (size_t)(&u));
}

template <class T>
using value_type_t = std::decay_t<decltype(std::declval<T>()[0])>;

template <class T> void _make_str(std::stringstream &ss, const T &t) {
  ss << t;
}

template <class T, class... Args>
void _make_str(std::stringstream &ss, const T &t, const Args &...args) {
  _make_str(ss, t);
  _make_str(ss, args...);
}

template <class... Args> std::string make_str(const Args &...args) {
  std::stringstream ss;
  _make_str(ss, args...);
  return ss.str();
}

template <class F> struct _defer_t {
  F func;
  _defer_t(F f) : func(f) {}
  ~_defer_t() { func(); }
};

inline result_t<std::string> get_ext(const std::string &file) {
  std::stack<char> ext_stk;

  bool has_dot = false;
  for (auto iter = file.rbegin(); iter != file.rend(); iter++) {
    if (*iter == '.') {
      has_dot = true;
      break;
    }

    ext_stk.push(*iter);
  }

  if (!has_dot) {
    return {"", make_str("cannot find a dot in filename: ", file,
                         ", unable to know the extension name")};
  }

  std::string ext;
  while (!ext_stk.empty()) {
    ext.push_back(ext_stk.top());
    ext_stk.pop();
  }

  return {ext, std::nullopt};
}

} // namespace common

#endif // COMMON_COMMON_HPP