#include <boost/dll.hpp>
#include <filesystem>
#include <spdlog/spdlog.h>

#include "./path.hpp"

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

int number_of_files(const path &dir) {
  return std::distance(dir_iterator(dir), dir_iterator{});
}
