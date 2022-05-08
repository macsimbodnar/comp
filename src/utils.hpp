#pragma once
#include "logger.hpp"
#include <string>

#define FAIL()                                                                 \
  do {                                                                         \
    std::cout << "Error at " << std::string(__func__) << " ("                  \
              << std::string(__FILE__) << ":" << std::to_string(__LINE__)      \
              << ")\n";                                                        \
    exit(EXIT_FAILURE);                                                        \
  } while (0);

namespace comp {

inline bool is_number(const std::string &s) {
  return std::all_of(s.begin(), s.end(),
                     [](char c) { return isdigit(c) != 0; });
}

} // namespace comp
