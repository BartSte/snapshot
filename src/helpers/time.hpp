#pragma once

#include <QTimer>
#include <chrono>
#include <cstdint>
#include <map>
#include <string>

extern const std::string valid_units[12];
extern std::map<char, std::chrono::duration<int64_t>> unit_vs_multiplier;

std::string timestamp();
std::string parseUnit(std::string str);
std::string parseNumber(std::string str);
std::chrono::seconds stringToSec(std::string str);

class ResetTimer : public QTimer {

  using ms = std::chrono::milliseconds;

  Q_OBJECT

 public:
  explicit ResetTimer(ms duration, ms interval = ms(1000),
                      QObject *parent = nullptr);
  void reset();
  ms getElapsed() { return elapsed; }

 private:
  ms elapsed;
  ms duration;
  void count();
  void check();

 signals:
  void timeout();
};
