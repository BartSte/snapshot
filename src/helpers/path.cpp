#include <cstddef>
#include <helpers/path.hpp>

#include <boost/dll.hpp>
#include <filesystem>
#include <regex>
#include <spdlog/spdlog.h>
#include <string>

using path = std::filesystem::path;
using dir_iterator = std::filesystem::directory_iterator;

const std::filesystem::path Path::program_location() {
  auto boost_path = boost::dll::program_location().parent_path();
  return std::filesystem::path(boost_path.string());
}

bool Path::mkdir(const std::filesystem::path &path) {
  if (std::filesystem::exists(path)) {
    return true;
  } else {
    spdlog::info("Creating directory {} to store frames.", path.string());
    return std::filesystem::create_directories(path);
  }
}

/**
 * @brief Returns the number of files/directories in a directory, non
 * recursively.
 *
 * @param dir the directory to count the number of items in
 *
 * @return the number of items in the directory
 */
std::size_t Path::numberOfItems(const path &dir) {
  if (!std::filesystem::exists(dir)) {
    spdlog::warn("Directory {} does not exist.", dir.string());
    return 0;
  } else {
    return std::distance(dir_iterator(dir), dir_iterator{});
  }
}

/**
 * @brief Recursively count the number of files in a directory, and all its
 * subdirectories.
 *
 * @param dir the directory to count the number of files in
 * @return the number of files in the directory
 */
std::size_t Path::numberOfFilesRecursive(const path &dir) {
  std::size_t count = 0;
  for (const auto &entry : dir_iterator(dir)) {
    if (entry.is_directory()) {
      count += Path::numberOfFilesRecursive(entry.path());
    } else {
      count++;
    }
  }
  return count;
}

/**
 * @brief returns the first submatch of a regex match as a string.
 *
 * @param match the match to extract the submatch from
 *
 * @param the first submatch as a string
 */
std::string first_submatch(const std::smatch &match) {
  for (std::size_t i = 1; i < match.size(); i++) {
    if (match[i].matched) {
      return match[i].str();
    }
  }
  throw std::runtime_error("No submatch found.");
}

/**
 * @brief Expands an environment variable.
 *
 * If the variable is not set, the empty string is returned.
 *
 * @param var The name of the environment variable to expand
 * @return the value of the environment variable
 */
std::string expand_env(std::string var) {
  // if var is a single ~ character, change it to the HOME.
  if (var == "~") var = "HOME";
  const char *s = getenv(var.c_str());
  return s == NULL ? "" : s;
}

/**
 * @brief Expands all environment variables in a string.
 *
 * Environment variables are of the form $VAR or ${VAR}. If the variable is not
 * set, the empty string is substituted. If the variable is set, the value of
 * the variable is substituted.
 *
 * @param str the string to expand
 * @return the expanded string
 */
std::string Path::expand(std::string str) {
  // regex: ${VAR} | $VAR | ~/ | ~
  // where the forward slash at ~/ is not captured in the match.
  static std::regex re_env("\\$\\{([^}]+)\\}|\\$(\\w+)|^(\\~)(?=\\/)|^(\\~)$");

  std::smatch match;
  while (std::regex_search(str, match, re_env)) {
    auto full = match[0];
    auto sub = first_submatch(match);
    str.replace(full.first, full.second, expand_env(sub));
  }

  return std::regex_replace(str, std::regex("//"), "/");
}

/**
 * @brief Expands all environment variables in a string.
 *
 * Calls expand(std::string) on the string.
 *
 * @param str the string to expand
 * @return the expanded string
 */
std::string Path::expand(const char *str) { return expand(std::string(str)); }

/**
 * @brief Expands all environment variables in a path.
 *
 * Calls expand(std::string) on the path's string representation.
 *
 * @param path the path to expand
 * @return the expanded path
 */
std::filesystem::path Path::expand(std::filesystem::path path) {
  return std::filesystem::path(expand(path.string()));
}
