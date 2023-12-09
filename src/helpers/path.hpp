#pragma once

#include <boost/dll.hpp>
#include <filesystem>

const std::filesystem::path program_location();
bool mkdir(const std::filesystem::path &path);
int number_of_files(const std::filesystem::path &dir);
