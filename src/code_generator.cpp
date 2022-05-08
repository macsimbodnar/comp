#include "code_generator.hpp"
#include "logger.hpp"
#include "utils.hpp"
#include <fstream>

namespace comp {

void code_generate(const std::string &out_file, const program_t &program) {
  std::ofstream stream(out_file, std::ios::out);

  if (!stream.is_open()) {
    std::cout << "Could not open the file - '" << out_file << "'" << std::endl;
    FAIL();
  }

  /**
   *
   * To generate a function (e.g. function “foo”):
   *
   *  .globl _foo
   * _foo:
   *  <FUNCTION BODY GOES HERE>
   *
   * To generate a return statement (e.g. return 3;):
   *
   *  movl    $3, %eax
   *  ret
   */

  stream << ".globl _" << program.function.name << "\n"
         << "_" << program.function.name << ":"
         << "\n"
         << "  pushq	%rbp"
         << "\n"
         << "  movq	%rsp, %rbp"
         << "\n"
         << "  movl	$" << program.function.statement.expression.value
         << ", %eax"
         << "\n"
         << "  popq	%rbp"
         << "\n"
         << "  retq"
         << "\n";
}

} // namespace comp
