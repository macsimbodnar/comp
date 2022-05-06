#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>

bool is_number(const std::string &s) {
  return std::all_of(s.begin(), s.end(),
                     [](char c) { return isdigit(c) != 0; });
}

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
    LITERAL_INTEGER
  };

  type_t type;
  int value_int;
  std::string value_str;

  token_t() = delete;
  token_t(type_t t) : type(t), value_int(0), value_str("") {}
  token_t(type_t t, int v) : type(t), value_int(v), value_str("") {}
  token_t(type_t t, std::string v) : type(t), value_int(0), value_str(v) {}
};

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

void print_token(const token_t &t) {

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

  default:
    break;
  }

  std::cout << pr << std::endl;
}

struct expression_t {
  int value;

  expression_t() = delete;
  expression_t(int v) : value(v) {}
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

int main(int argc, char *argv[]) {
  std::string file = std::string(argv[1]);

  std::cout << "Compiling " << file << std::endl;

  std::queue<token_t> tokens = lex(file);

  {
    std::queue<token_t> q = tokens;
    std::cout << "Found " << q.size() << " tokens" << std::endl;
    while (!q.empty()) {
      auto I = q.front();
      print_token(I);
      q.pop();
    }
  }

  program_t program = parse_program(tokens);

  return 0;
}