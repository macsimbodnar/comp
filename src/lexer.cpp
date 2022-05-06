#include "lexer.hpp"
#include "logger.hpp"
#include "utils.hpp"
#include <fstream>

namespace comp {

void get_token(std::string &bytes, std::queue<token_t> &result) {
  if (!bytes.empty()) {
    if (bytes == "int") {
      result.emplace(token_t::KEYWORD_INT);
    } else if (bytes == "return") {
      result.emplace(token_t::KEYWORD_RETURN);
    } else if (is_number(bytes)) {
      result.emplace(token_t::LITERAL_INTEGER, std::stoi(bytes));
    } else {
      result.emplace(token_t::IDENTIFIER, bytes);
    }

    bytes.clear();
  }
}

std::queue<token_t> lex(const std::string &file) {
  std::queue<token_t> result;

  std::ifstream stream(file, std::ios::in);

  if (!stream.is_open()) {
    std::cout << "Could not open the file - '" << file << "'" << std::endl;
    exit(EXIT_FAILURE);
  }

  /**
   *   Open brace {
   *   Close brace }
   *   Open parenthesis \(
   *   Close parenthesis \)
   *   Semicolon ;
   *
   *   Int keyword int
   *   Return keyword return
   *   Identifier [a-zA-Z]\w*
   *   Integer literal [0-9]+
   */

  std::string bytes;
  char c = 0;

  // Negation -
  // Bitwise complement ~
  // Logical negation !

  while (stream.get(c)) {

    switch (c) {
    case '{':
      get_token(bytes, result);
      result.emplace(token_t::BRACES_OPEN);
      break;

    case '}':
      get_token(bytes, result);
      result.emplace(token_t::BRACES_CLOSE);
      break;

    case '(':
      get_token(bytes, result);
      result.emplace(token_t::PARENTHESIS_OPEN);
      break;

    case ')':
      get_token(bytes, result);
      result.emplace(token_t::PARENTHESIS_CLOSE);
      break;

    case ';':
      get_token(bytes, result);
      result.emplace(token_t::SEMICOLON);
      break;

    case '-':
      get_token(bytes, result);
      result.emplace(token_t::NEGATION);
      break;

    case '~':
      get_token(bytes, result);
      result.emplace(token_t::BITWISE_COMPLEMENT);
      break;

    case '!':
      get_token(bytes, result);
      result.emplace(token_t::LOGICAL_NEGATION);
      break;

    case ' ':
    case '\n':
      get_token(bytes, result);
      break;

    default:
      bytes.push_back(c);
      continue;
    }
  }

  return result;
}

} // namespace comp
