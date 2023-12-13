#include <iostream>
#include <string>

struct CoutRedirect {
  explicit CoutRedirect(std::streambuf *new_buffer);
  ~CoutRedirect();

 private:
  std::streambuf *old;
};
