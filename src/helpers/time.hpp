#include <chrono>
#include <cstdint>
#include <map>
#include <string>

extern const std::string valid_units[12];
extern std::map<char, std::chrono::duration<int64_t>> unit_vs_multiplier;

std::string parseUnit(std::string str);
std::string parseNumber(std::string str);
std::chrono::milliseconds stringToMs(std::string str);
