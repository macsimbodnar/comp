#pragma once

namespace comp {

inline bool is_number(const std::string &s) {
  return std::all_of(s.begin(), s.end(),
                     [](char c) { return isdigit(c) != 0; });
}

} // namespace comp
