#include "lexer.h"
#include <iostream>

auto main() -> int {
  static const char *source = "\"hello\" begin tagline company experience";
  papy::Lexer lexer(source);

  auto tokens = lexer.lex();

  std::cout << tokens.size() << " token(s)\n";

  for (auto token: tokens) {
    std::cout << token.lexeme << "\n";
  }

  for (auto diag: lexer.diagnostics) {
    std::cerr << diag.lineno << "|" << diag.message << "\n";
  }

  return 0;
}
