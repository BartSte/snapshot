#include <helpers/time.hpp>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <map>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <string>

using sec = std::chrono::seconds;

const std::string valid_units[12] = {"s",       "m",       "h",     "d",
                                     "second",  "minute",  "hour",  "day",
                                     "seconds", "minutes", "hours", "days"};

std::map<char, std::chrono::duration<int64_t>> unit_vs_multiplier = {
    {'s', std::chrono::seconds(1)},
    {'m', std::chrono::minutes(1)},
    {'h', std::chrono::hours(1)},
    {'d', std::chrono::hours(24)}};

/**
 * @brief timestamp
 *
 * Returns a string representing the current time in the following format:
 * <day>-<month>-<year>_<hour>-<minute>-<second> (e.g. 01-01-2021_12-00-00).
 *
 * @return a string representing the current time.
 */
std::string timestamp() {
  time_t rawtime;
  struct tm *timeinfo;
  char buffer[80];

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(buffer, sizeof(buffer), "%d-%m-%Y_%H-%M-%S", timeinfo);
  std::string str(buffer);

  return str;
}

/**
 * @brief isNumber
 *
 * Returns true if the string consists only of a number. Positive integers are
 * supported.
 *
 * @param str
 * @return
 */
bool isNumber(const std::string &str) {
  return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

/**
 * @brief hasForbiddenChars
 *
 * Returns true if the string contains any of the following characters: .,;:?!-
 *
 * @param str
 * @return
 */
bool hasForbiddenChars(const std::string &str) {
  return str.find_first_of(".,;:?!-") != std::string::npos;
}

/**
 * @brief check
 *
 * Checks if the string is valid. If it is not, an exception is thrown.
 *
 * @param str
 */
void check(std::string str) {
  if (str.empty()) {
    throw std::invalid_argument("Empty string");
  } else if (hasForbiddenChars(str)) {
    throw std::invalid_argument("Invalid character found: " + str);
  }
}

/**
 * @brief Convert a string to a duration in seconds.
 *
 * @param timeString A string representing a time duration. The following
 * formats are valid:
 * - A number followed by a unit (e.g. 10s or 5minutes). The following units
 *   are supported: second, seconds, minute, minutes, hour, hours, day, days.
 *   Their abbreviations are also supported: s, m, h, d, respectively.
 * - White space (tabs and spaces) are ignored.
 * - All other formats are invalid and will throw an exception, including
 *   perionds and commas.
 *
 * @return the number of seconds.
 */
sec stringToSec(std::string str) {
  str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());
  check(str);

  if (isNumber(str)) {
    return sec(std::stoi(str));

  } else {
    std::string unit = parseUnit(str);
    std::string number = parseNumber(str);
    sec multiplier = unit_vs_multiplier[unit.front()];
    return sec(std::stoll(number) * multiplier.count());
  }
}

/**
 * @brief parseUnit
 *
 * Recievs a string in the form of: <number><unit> where <number> consists only
 * of digits and <unit> is one of the following: s, m, h, d, second, minute,
 * hour, day, seconds, minutes, hours, days. This function return the <unit>
 * part of the string. If the unit is not one of the above, an exception is
 * thrown.
 *
 * @param str the string containing <number><unit>
 * @return the <unit> part of the string.
 */
std::string parseUnit(std::string str) {
  str.erase(std::remove_if(str.begin(), str.end(), isdigit), str.end());
  for (std::string unit : valid_units) {
    if (str == unit) {
      return unit;
    }
  }
  throw std::invalid_argument("Invalid unit: " + str);
}

/**
 * @brief parseNumber
 *
 * Recievs a string in the form of: <number><unit> where <number> consists only
 * of digits and <unit> describes the time unit (see parseUnit). This function
 * returns the <number> part of the string. If the number is not a valid number,
 * an exception is thrown.
 *
 * @param str the string containing <number><unit>
 * @return the <number> part of the string.
 */
std::string parseNumber(std::string str) {
  str.erase(std::remove_if(str.begin(), str.end(), isalpha), str.end());
  if (isNumber(str)) {
    return str;
  } else {
    throw std::invalid_argument("Invalid number: " + str);
  }
}

/**
 * @brief ResetTimer::ResetTimer
 *
 * A timer that can be reset.
 *
 * @param parent
 */
ResetTimer::ResetTimer(ms duration, ms interval, QObject *parent)
    : QTimer(parent), duration(duration), elapsed(ms(0)) {
  setInterval(interval);
  connect(this, &QTimer::timeout, this, &ResetTimer::count);
  connect(this, &QTimer::timeout, this, &ResetTimer::check);
}

/**
 * @brief ResetTimer::count
 *
 * Increments the elapsed time by the timer's interval.
 */
void ResetTimer::count() { elapsed += intervalAsDuration(); }

/**
 * @brief ResetTimer::check
 *
 * Checks if the timer's elapsed time exceeds the duration. If it does, the
 * timer is stopped and a ResetTimer::timeout signal is emitted.
 */
void ResetTimer::check() {
  if (elapsed >= duration) {
    spdlog::info("The video connection was lost for {} seconds, exceeding the "
                 "timeout of {} seconds",
                 elapsed.count(), duration.count());
    stop();
    emit this->timeout();
  }
}

/**
 * @brief ResetTimer::reset
 *
 * Resets the timer's elapsed time to 0.
 */
void ResetTimer::reset() { elapsed = ms(0); }
