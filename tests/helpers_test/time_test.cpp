#include <gtest/gtest.h>
#include <helpers/time.hpp>
#include <vector>

using ms = std::chrono::milliseconds;

extern const std::string valid_units[12];
extern std::map<char, std::chrono::duration<int64_t>> unit_vs_multiplier;

TEST(timeTest, stringToMs) {
  std::chrono::duration<int64_t> multiplier;
  std::string strings[] = {"1", " 1 ", "001"};
  for (auto unit : valid_units) {
    for (auto str : strings) {
      multiplier = unit_vs_multiplier[unit.front()];
      ASSERT_EQ(stringToMs(str + unit), ms(std::stoi(str) * multiplier));
    }
  }
}

TEST(timeTest, stringToMsNoUnit) {
  std::string strings[] = {"1", " 1 ", "123", "001"};
  for (std::string str : strings) {
    ms actual = stringToMs(str);
    ASSERT_EQ(actual, ms(std::stoi(str)));
  }
}

TEST(timeTest, stringToMsEmptyString) {
  try {
    stringToMs("");
    FAIL();
  } catch (std::invalid_argument &error) {
    SUCCEED();
  } catch (...) {
    FAIL();
  }
}

TEST(timeTest, stringToMsInvalidString) {
  std::string strings[] = {"a", "1a", "a1", "1dayss"};
  for (auto str : strings) {
    try {
      stringToMs(str);
      FAIL();
    } catch (std::invalid_argument &error) {
      SUCCEED();
    } catch (...) {
      FAIL();
    }
  }
}

TEST(timeTest, stringToMsDecimals) {
  std::string strings[] = {"1.2", " 1.2 ", "001.2"};
  for (auto unit : valid_units) {
    for (auto str : strings) {
      try {
        stringToMs(str + unit);
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

TEST(timeTest, parseUnitValid) {
  std::string numbers[] = {"1", "123"};
  for (auto number : numbers) {
    assertWithAllUnits(number);
  }
}

TEST(timeTest, parseUnitInvalid) {
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

TEST(timeTest, parseNumberValid) {
  std::string strings[] = {"1s", "2m", "3hours", "4days", "5abcd", "1.2h"};
  std::string expected[] = {"1", "2", "3", "4", "5", "1.2"};
  for (int i = 0; i < 5; i++) {
    std::string actual = parseNumber(strings[i]);
    ASSERT_EQ(actual, expected[i]);
  }
}

TEST(timeTest, parseNumberInvalid) {
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
