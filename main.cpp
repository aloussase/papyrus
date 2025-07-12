#include "Parser.h"
#include <iostream>
#include <memory>

#include "PdfBackend.h"

auto main() -> int {
  static const char *source = R"(begin metadata
  name "Alexander Goussas "
  email "goussasalexander@gmail.com"
  phone "0995764993"
  tagline "Software Developer"
end

begin education
  institution "ESPOL"
  degree "Computer Science"
  begin activities
    activity "hello world!"
    activity "man that sucks"
  end
end
)";

  papy::Parser parser(source);
  auto ast = parser.parse();

  auto backend = std::make_unique<papy::PdfBackend>();

  if (!ast.empty()) {
    backend->accept(ast);
    for (auto node: ast) {
      delete node;
    }
    return 0;
  } else {
    for (const auto &diag: parser.m_diagnostics) {
      std::cerr << diag.lineno << "|" << diag.message << "\n";
    }
    return 1;
  }
}
