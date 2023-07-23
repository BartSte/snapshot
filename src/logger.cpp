#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include <spdlog/spdlog.h>
#include <string>

void setLogger(std::string level, std::string pattern) {
  std::transform(level.begin(), level.end(), level.begin(), ::tolower);
  spdlog::set_level(spdlog::level::from_str(level));
  spdlog::set_pattern(pattern);
  SPDLOG_INFO("Loglevel set to {}", level);
  SPDLOG_DEBUG("Debug logging enabled");
}
