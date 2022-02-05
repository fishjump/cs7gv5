#ifndef COMMON_LOG_HPP
#define COMMON_LOG_HPP

#include <iostream>
#include <sstream>

#define LOG_LEVEL_NO_LOG 0
#define LOG_LEVEL_ERR 1
#define LOG_LEVEL_WRAN 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_DEBUG 4

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_INFO
#endif

#define LOG_STR(...)                                                           \
  common::make_str("[", __FUNCTION__, "] at ", __FILE__, ":", __LINE__, " ",   \
                   __VA_ARGS__)
#define LOG_ERR_STR(...) common::make_str("[ERR]", LOG_STR(__VA_ARGS__))
#define LOG_WRAN_STR(...) common::make_str("[WRAN]", LOG_STR(__VA_ARGS__))
#define LOG_INFO_STR(...) common::make_str("[INFO]", LOG_STR(__VA_ARGS__))
#define LOG_DEBUG_STR(...) common::make_str("[DEBUG]", LOG_STR(__VA_ARGS__))

#define LOG(log_level, msg)                                                    \
  do {                                                                         \
    if constexpr (LOG_LEVEL >= log_level) {                                    \
      std::cerr << msg << std::endl;                                           \
    }                                                                          \
  } while (false);

#define LOG_ERR(...)                                                           \
  do {                                                                         \
    LOG(LOG_LEVEL_ERR, LOG_ERR_STR(__VA_ARGS__));                              \
  } while (false);

#define LOG_WRAN(...)                                                          \
  do {                                                                         \
    LOG(LOG_LEVEL_WRAN, LOG_WRAN_STR(__VA_ARGS__));                            \
  } while (false);

#define LOG_INFO(...)                                                          \
  do {                                                                         \
    LOG(LOG_LEVEL_INFO, LOG_INFO_STR(__VA_ARGS__));                            \
  } while (false);

#define LOG_DEBUG(...)                                                         \
  do {                                                                         \
    LOG(LOG_LEVEL_DEBUG, LOG_DEBUG_STR(__VA_ARGS__));                          \
  } while (false);

#endif // COMMON_LOG_HPP