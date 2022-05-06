#pragma once
#include "token.hpp"
#include <queue>
#include <string>

namespace comp {

std::queue<token_t> lex(const std::string &file);

}
