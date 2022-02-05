#ifndef CS7GV5_PROGRAM_OPTIONS_HPP
#define CS7GV5_PROGRAM_OPTIONS_HPP

#include <optional>

#include <boost/program_options.hpp>

namespace cs7gv5 {

struct program_options_t {
  std::optional<std::string> vert = std::nullopt;
  std::optional<std::string> frag = std::nullopt;
  std::optional<std::string> model = std::nullopt;
};

const program_options_t &parse_opts(int argc, char *argv[]);

} // namespace cs7gv5

#endif // CS7GV5_PROGRAM_OPTIONS_HPP