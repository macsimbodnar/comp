#pragma once

#include "logger.hpp"
#include "utils.hpp"
#include <memory>
#include <unordered_map>

namespace comp {

/**
 *   Open brace {
 *   Close brace }
 *   Open parenthesis \(
 *   Close parenthesis \)
 *   Semicolon ;
 *   Int keyword int
 *   Return keyword return
 *   Identifier [a-zA-Z]\w*
 *   Integer literal [0-9]+
 */
struct token_t {
  enum type_t {
    BRACES_OPEN,
    BRACES_CLOSE,
    PARENTHESIS_OPEN,
    PARENTHESIS_CLOSE,
    SEMICOLON,
    KEYWORD_INT,
    KEYWORD_RETURN,
    IDENTIFIER,
    LITERAL_INTEGER,
    NEGATION,
    BITWISE_COMPLEMENT,
    LOGICAL_NEGATION
  };

  type_t type;
  int value_int;
  std::string value_str;

  token_t() = delete;
  token_t(type_t t) : type(t), value_int(0), value_str("") {}
  token_t(type_t t, int v) : type(t), value_int(v), value_str("") {}
  token_t(type_t t, std::string v) : type(t), value_int(0), value_str(v) {}
};

struct unary_operation_t {
  enum operator_t { NEGATON, BITWISE_COMPLEMENT, LOGICAL_NEGATION };

  operator_t opr;

  unary_operation_t() = delete;
  unary_operation_t(operator_t o) : opr(o) {}

  static operator_t get_operator(token_t::type_t t) {
    switch (t) {
    case token_t::NEGATION:
      return operator_t::NEGATON;

    case token_t::BITWISE_COMPLEMENT:
      return operator_t::BITWISE_COMPLEMENT;

    case token_t::LOGICAL_NEGATION:
      return operator_t::LOGICAL_NEGATION;

    default:
      FAIL();
    }
  }
};

struct expression_t {

  struct unary_op_expression_t {
    expression_t expression;
    unary_operation_t opr;

    unary_op_expression_t() = delete;
    unary_op_expression_t(expression_t e, unary_operation_t o)
        : expression(e), opr(o) {}
  };

  int value;
  std::unique_ptr<unary_op_expression_t> unary_op;

  expression_t() = delete;
  expression_t(int v) : value(v) {}
  expression_t(expression_t e, unary_operation_t o)
      : value(0), unary_op(std::make_unique<unary_op_expression_t>(e, o)) {}
};

struct statement_t {
  expression_t expression;

  statement_t() = delete;
  statement_t(expression_t &e) : expression(e) {}
};

struct function_t {
  std::string name;
  statement_t statement;

  function_t() = delete;
  function_t(const std::string &str, const statement_t &s)
      : name(str), statement(s) {}
};

struct program_t {
  function_t function;

  program_t() = delete;
  program_t(const function_t &f) : function(f) {}
};

struct node_t {
  enum type_t { EXPPRESSIONS, STATEMENT, FUNCTION, PROGRAM };

  type_t type;
};

inline void print_token(const token_t &t) {

  std::string pr;

  switch (t.type) {

  case token_t::BRACES_OPEN:
    pr = "BRACES_OPEN             {";
    break;
  case token_t::BRACES_CLOSE:
    pr = "BRACES_CLOSE            }";
    break;
  case token_t::PARENTHESIS_OPEN:
    pr = "PARENTHESIS_OPEN        (";
    break;
  case token_t::PARENTHESIS_CLOSE:
    pr = "PARENTHESIS_CLOSE       )";
    break;
  case token_t::SEMICOLON:
    pr = "SEMICOLON               ;";
    break;
  case token_t::KEYWORD_INT:
    pr = "KEYWORD_INT             int";
    break;
  case token_t::KEYWORD_RETURN:
    pr = "KEYWORD_RETURN          return";
    break;
  case token_t::IDENTIFIER:
    pr = "IDENTIFIER              " + t.value_str;
    break;
  case token_t::LITERAL_INTEGER:
    pr = "LITERAL_INTEGER         " + std::to_string(t.value_int);
    break;
  case token_t::NEGATION:
    pr = "NEGATION                -";
    break;
  case token_t::BITWISE_COMPLEMENT:
    pr = "BITWISE_COMPLEMENT      ~";
    break;
  case token_t::LOGICAL_NEGATION:
    pr = "LOGICAL_NEGATION         !";
    break;

  default:
    break;
  }

  std::cout << pr << std::endl;
}

} // namespace comp
