#include "code_generator.hpp"
#include "lexer.hpp"
#include "logger.hpp"
#include "parser.hpp"
#include "token.hpp"
#include <queue>
#include <string>

using namespace comp;

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

  code_generate("assembly.s", program);

  return 0;
}
