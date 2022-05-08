#include "parser.hpp"
#include "utils.hpp"
#include <string>

namespace comp {
// def parse_expression(tokens):
//     tok = tokens.next()
//     if tok.type == "INT":
//         //parse this the same way as before, return a Const node
//     else:
//         op = get_operator(tok) //convert token to unary_op AST element - fail if token isn't "!", "~" or "-" 
//         inner_exp  = parse_expression(tokens)
//         //HOORAY, RECURSION - this will pop more tokens off the stack return
//         UnOp(op, inner_exp)


expression_t parse_expression(std::queue<token_t> &tokens) {
  token_t t = tokens.front();
  tokens.pop();

  if (t.type == token_t::LITERAL_INTEGER) {
    return expression_t(t.value_int);
  }

  unary_operation_t::operator_t op = unary_operation_t::get_operator(t.type);
  expression_t inner_expression = parse_expression(tokens);

  return expression_t(inner_expression, op);
}

statement_t parse_statement(std::queue<token_t> &tokens) {

  {
    token_t t = tokens.front();
    tokens.pop();

    if (t.type != token_t::KEYWORD_RETURN) {
      FAIL();
    }
  }

  expression_t expression = parse_expression(tokens);
  statement_t result(expression);

  {
    token_t t = tokens.front();
    tokens.pop();

    if (t.type != token_t::SEMICOLON) {
      FAIL();
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
      FAIL();
    }
  }

  {
    token_t t = tokens.front();
    tokens.pop();

    if (t.type != token_t::IDENTIFIER) {
      FAIL();
    }

    function_name = t.value_str;
  }

  {
    token_t t = tokens.front();
    tokens.pop();

    if (t.type != token_t::PARENTHESIS_OPEN) {
      FAIL();
    }
  }

  {
    token_t t = tokens.front();
    tokens.pop();

    if (t.type != token_t::PARENTHESIS_CLOSE) {
      FAIL();
    }
  }

  {
    token_t t = tokens.front();
    tokens.pop();

    if (t.type != token_t::BRACES_OPEN) {
      FAIL();
    }
  }

  statement_t statement = parse_statement(tokens);

  {
    token_t t = tokens.front();
    tokens.pop();

    if (t.type != token_t::BRACES_CLOSE) {
      FAIL();
    }
  }

  return function_t(function_name, statement);
}

program_t parse_program(std::queue<token_t> &tokens) {
  return parse_function(tokens);
}

} // namespace comp
