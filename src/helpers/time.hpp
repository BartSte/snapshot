#include <chrono>
#include <cstdint>
#include <string>

extern const std::string valid_units[12];

void check(std::string str);
bool isNumber(const std::string &s);
std::string parseUnit(std::string str);
std::string parseNumber(std::string str);
std::chrono::milliseconds string_to_milliseconds(std::string str);
