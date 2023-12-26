#pragma once

#include <boost/dll.hpp>
#include <cstddef>
#include <filesystem>
#include <string>

namespace Path {
const std::filesystem::path program_location();
bool mkdir(const std::filesystem::path &path);
std::size_t numberOfItems(const std::filesystem::path &dir);
std::size_t numberOfFilesRecursive(const std::filesystem::path &dir);
std::string expand(std::string str);
std::string expand(const char *str);
std::filesystem::path expand(std::filesystem::path path);
} // namespace Path
