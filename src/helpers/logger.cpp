#include <spdlog/spdlog.h>
#include <string>

#include "./helpers/logger.hpp"

/**
 * @brief set
 *
 * Set the loglevel and the pattern of the logger.
 *
 * @param level The loglevel. Options are: trace, debug, info, warn, error,
 * critical.
 * @param pattern The pattern of the logger. See spdlog documentation for
 * details.
 */
void logging::set(std::string level, std::string pattern) {
  std::transform(level.begin(), level.end(), level.begin(), ::tolower);
  spdlog::set_level(spdlog::level::from_str(level));
  spdlog::set_pattern(pattern);
  spdlog::debug("Loglevel: {}", level);
  spdlog::debug("Pattern: {}", pattern);
}
