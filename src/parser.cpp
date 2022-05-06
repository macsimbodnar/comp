#include "parser.hpp"
#include <string>

namespace comp {

// def parse_statement(tokens):
//     tok = tokens.next()
//     if tok.type != "RETURN_KEYWORD":
//         fail()
//     tok = tokens.next()
//     if tok.type != "INT"
//         fail()
//     exp = parse_exp(tokens) //parse_exp will pop off more tokens
//     statement = Return(exp)

//     tok = tokens.next()
//     if tok.type != "SEMICOLON":
//         fail()

//     return statement

expression_t parse_expression(std::queue<token_t> &tokens) {
  token_t t = tokens.front();
  tokens.pop();

  if (t.type != token_t::LITERAL_INTEGER) {
    exit(EXIT_FAILURE);
  }

  return expression_t(t.value_int);
}

statement_t parse_statement(std::queue<token_t> &tokens) {

  {
    token_t t = tokens.front();
    tokens.pop();

    if (t.type != token_t::KEYWORD_RETURN) {
      exit(EXIT_FAILURE);
    }
  }

  expression_t expression = parse_expression(tokens);
  statement_t result(expression);

  {
    token_t t = tokens.front();
    tokens.pop();

    if (t.type != token_t::SEMICOLON) {
      exit(EXIT_FAILURE);
    }
  }

  return result;
}

/**
 * <program> ::= <function>
 * <function> ::= "int" <id> "(" ")" "{" <statement> "}"
 * <statement> ::= "return" <exp> ";"
 * <exp> ::= <int>
 */
function_t parse_function(std::queue<token_t> &tokens) {
  std::string function_name;

  {
    token_t t = tokens.front();
    tokens.pop();

    if (t.type != token_t::KEYWORD_INT) {
      exit(EXIT_FAILURE);
    }
  }

  {
    token_t t = tokens.front();
    tokens.pop();

    if (t.type != token_t::IDENTIFIER) {
      exit(EXIT_FAILURE);
    }

    function_name = t.value_str;
  }

  {
    token_t t = tokens.front();
    tokens.pop();

    if (t.type != token_t::PARENTHESIS_OPEN) {
      exit(EXIT_FAILURE);
    }
  }

  {
    token_t t = tokens.front();
    tokens.pop();

    if (t.type != token_t::PARENTHESIS_CLOSE) {
      exit(EXIT_FAILURE);
    }
  }

  {
    token_t t = tokens.front();
    tokens.pop();

    if (t.type != token_t::BRACES_OPEN) {
      exit(EXIT_FAILURE);
    }
  }

  statement_t statement = parse_statement(tokens);

  {
    token_t t = tokens.front();
    tokens.pop();

    if (t.type != token_t::BRACES_CLOSE) {
      exit(EXIT_FAILURE);
    }
  }

  return function_t(function_name, statement);
}

program_t parse_program(std::queue<token_t> &tokens) {
  return parse_function(tokens);
}

} // namespace comp
