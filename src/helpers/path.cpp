#include <cstddef>
#include <helpers/path.hpp>

#include <boost/dll.hpp>
#include <filesystem>
#include <spdlog/spdlog.h>

using path = std::filesystem::path;
using dir_iterator = std::filesystem::directory_iterator;

const std::filesystem::path program_location() {
  auto boost_path = boost::dll::program_location().parent_path();
  return std::filesystem::path(boost_path.string());
}

bool mkdir(const std::filesystem::path &path) {
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
std::size_t numberOfItems(const path &dir) {
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
std::size_t numberOfFilesRecursive(const path &dir) {
  std::size_t count = 0;
  for (const auto &entry : dir_iterator(dir)) {
    if (entry.is_directory()) {
      count += numberOfFilesRecursive(entry.path());
    } else {
      count++;
    }
  }
  return count;
}
