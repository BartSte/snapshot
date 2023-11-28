#include <gtest/gtest.h>
#include <helpers/time.hpp>

using ms = std::chrono::milliseconds;

TEST(timeTest, millisecondsNoUnit) {
  std::string strings[] = {"1", " 1 ", "123"};
  for (auto str : strings) {
    try {
      string_to_milliseconds(str);
    } catch (std::invalid_argument &error) {
      ASSERT_STREQ(error.what(), "Number without unit");
    }
  }
}

TEST(timeTest, millisecondsSeconds) {
  std::string strings[] = {"1s", " 1s "};
  for (auto str : strings) {
    ms actual = string_to_milliseconds(str);
    ms expected(1000);
    ASSERT_EQ(actual, expected);
  }
}

TEST(timeTest, millisecondsMinutes) {
  std::string strings[] = {"1m", " 1m "};
  for (auto str : strings) {
    ms actual = string_to_milliseconds(str);
    ms expected(1000 * 60);
    ASSERT_EQ(actual, expected);
  }
}

TEST(timeTest, millisecondsHours) {
  std::string strings[] = {"1h", " 1h "};
  for (auto str : strings) {
    ms actual = string_to_milliseconds(str);
    ms expected(1000 * 60 * 60);
    ASSERT_EQ(actual, expected);
  }
}

TEST(timeTest, millisecondsDays) {
  std::string strings[] = {"1d", " 1d "};
  for (auto str : strings) {
    ms actual = string_to_milliseconds(str);
    ms expected(1000 * 60 * 60 * 24);
    ASSERT_EQ(actual, expected);
  }
}

TEST(timeTest, millisecondsEmptyString) {
  try {
    string_to_milliseconds("");
  } catch (std::invalid_argument &error) {
    ASSERT_STREQ(error.what(), "Empty string");
  }
}

TEST(timeTest, millisecondsInvalidString) {
  std::string strings[] = {"a", "1a", "a1", "1dayss"};
  for (auto str : strings) {
    try {
      string_to_milliseconds(str);
    } catch (std::invalid_argument &error) {
      ASSERT_STREQ(error.what(), "Invalid unit");
    }
  }
}

void assertWithAllUnits(std::string number) {
  std::string str;
  extern const std::string valid_units[12];
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
      ASSERT_STREQ(error.what(), "Invalid unit");
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
      ASSERT_STREQ(error.what(), "Invalid number");
    } catch (...) {
      FAIL();
    }
  }
}
