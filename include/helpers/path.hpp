#pragma once

#include <boost/dll.hpp>
#include <cstddef>
#include <filesystem>

const std::filesystem::path program_location();
bool mkdir(const std::filesystem::path &path);
std::size_t numberOfItems(const std::filesystem::path &dir);
std::size_t numberOfFilesRecursive(const std::filesystem::path &dir);
