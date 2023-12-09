#include <chrono>
#include <gtest/gtest.h>
#include <helpers/time.hpp>
#include <qcoreapplication.h>
#include <qthread.h>
#include <spdlog/spdlog.h>
#include <thread>
#include <vector>

using ms = std::chrono::milliseconds;
using sec = std::chrono::seconds;

extern const std::string valid_units[12];
extern std::map<char, std::chrono::duration<int64_t>> unit_vs_multiplier;

TEST(testTime, stringToSec) {
  std::chrono::duration<int64_t> multiplier;
  std::string strings[] = {"1", " 1 ", "001"};
  for (auto unit : valid_units) {
    for (auto str : strings) {
      multiplier = unit_vs_multiplier[unit.front()];
      ASSERT_EQ(stringToSec(str + unit), sec(std::stoi(str) * multiplier));
    }
  }
}

TEST(testTime, stringToSecNoUnit) {
  std::string strings[] = {"1", " 1 ", "123", "001"};
  for (std::string str : strings) {
    sec actual = stringToSec(str);
    ASSERT_EQ(actual, sec(std::stoi(str)));
  }
}

TEST(testTime, stringToSecEmptyString) {
  try {
    stringToSec("");
    FAIL();
  } catch (std::invalid_argument &error) {
    SUCCEED();
  } catch (...) {
    FAIL();
  }
}

TEST(testTime, stringToSecInvalidString) {
  std::string strings[] = {"a", "1a", "a1", "1dayss"};
  for (auto str : strings) {
    try {
      stringToSec(str);
      FAIL();
    } catch (std::invalid_argument &error) {
      SUCCEED();
    } catch (...) {
      FAIL();
    }
  }
}

TEST(testTime, stringToSecDecimals) {
  std::string strings[] = {"1.2", " 1.2 ", "001.2"};
  for (auto unit : valid_units) {
    for (auto str : strings) {
      try {
        stringToSec(str + unit);
        FAIL();
      } catch (std::invalid_argument &error) {
        SUCCEED();
      } catch (...) {
        FAIL();
      }
    }
  }
}

void assertWithAllUnits(std::string number) {
  std::string str;
  for (std::string unit : valid_units) {
    str = parseUnit(number + unit);
    ASSERT_EQ(str, unit);
  }
}

TEST(testTime, parseUnitValid) {
  std::string numbers[] = {"1", "123"};
  for (auto number : numbers) {
    assertWithAllUnits(number);
  }
}

TEST(testTime, parseUnitInvalid) {
  std::string numbers[] = {"a", "1a", "a1", "1dayss"};
  for (auto number : numbers) {
    try {
      parseUnit(number);
      FAIL();
    } catch (std::invalid_argument &error) {
      SUCCEED();
    } catch (...) {
      FAIL();
    }
  }
}

TEST(testTime, parseNumberValid) {
  std::string strings[] = {"1s", "2m", "3hours", "4days", "5abcd", "1.2h"};
  std::string expected[] = {"1", "2", "3", "4", "5", "1.2"};
  for (int i = 0; i < 5; i++) {
    std::string actual = parseNumber(strings[i]);
    ASSERT_EQ(actual, expected[i]);
  }
}

TEST(testTime, parseNumberInvalid) {
  std::string strings[] = {"", "s", "m", "hours", "days", "abcd"};
  for (auto str : strings) {
    try {
      parseNumber(str);
      FAIL();
    } catch (std::invalid_argument &error) {
      SUCCEED();
    } catch (...) {
      FAIL();
    }
  }
}

TEST(testTime, testResetTimer) {
  ResetTimer timer(ms(100), ms(10));
  bool called = false;
  auto callback = [&called]() { called = true; };
  timer.connect(&timer, &ResetTimer::timeout, callback);

  timer.start();
  ASSERT_EQ(timer.getElapsed().count(), ms(0).count());

  std::this_thread::sleep_for(ms(10));
  QCoreApplication::processEvents();
  ASSERT_EQ(timer.getElapsed().count(), ms(10).count());

  timer.reset();
  ASSERT_EQ(timer.getElapsed().count(), ms(0).count());

  int cnt = 0;
  while (cnt < 10) {
    std::this_thread::sleep_for(ms(10));
    QCoreApplication::processEvents();
    ++cnt;
  }
  ASSERT_EQ(timer.getElapsed().count(), ms(100).count());
  ASSERT_TRUE(called);
}
