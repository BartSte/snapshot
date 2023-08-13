#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include <spdlog/spdlog.h>
#include <string>

#include "./helpers/logger.hpp"


void logging::set(std::string level, std::string pattern) {
  std::transform(level.begin(), level.end(), level.begin(), ::tolower);
  spdlog::set_level(spdlog::level::from_str(level));
  spdlog::set_pattern(pattern);
  SPDLOG_DEBUG("Loglevel: {}", level);
  SPDLOG_DEBUG("Pattern: {}", pattern);
}
