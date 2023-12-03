#include <boost/dll.hpp>
#include <filesystem>
#include <spdlog/spdlog.h>

#include "./helpers/path.hpp"

const std::filesystem::path program_location() {
  auto boost_path = boost::dll::program_location().parent_path();
  return std::filesystem::path(boost_path.string());
}

bool mkdir(const std::filesystem::path &path) {
  if (std::filesystem::exists(path)) {
    return true;
  } else {
    spdlog::warn("Creating directory {} to store frames.", path.string());
    return std::filesystem::create_directories(path);
  }
}
