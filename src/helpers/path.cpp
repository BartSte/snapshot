#include <boost/dll.hpp>
#include <filesystem>

#include "./helpers/path.hpp"

const std::filesystem::path program_location() {
  auto boost_path = boost::dll::program_location().parent_path();
  return std::filesystem::path(boost_path.string());
}
