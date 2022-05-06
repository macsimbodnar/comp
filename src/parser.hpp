#pragma once

#include "token.hpp"
#include <queue>

namespace comp {

program_t parse_program(std::queue<token_t> &tokens);

}
