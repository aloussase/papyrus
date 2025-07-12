#include "Parser.h"
#include <iostream>

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

  if (ast.size() > 0 && ast[0]) {
    std::cout << dynamic_cast<papy::ast::Metadata *>(ast[0])->m_name << "\n";
    std::cout << dynamic_cast<papy::ast::Metadata *>(ast[0])->m_email << "\n";
    std::cout << dynamic_cast<papy::ast::Metadata *>(ast[0])->m_phone_number << "\n";
    std::cout << dynamic_cast<papy::ast::Metadata *>(ast[0])->m_tagline << "\n";
    delete ast[0];
  }

  if (ast.size() >= 1 && ast[1]) {
    std::cout << dynamic_cast<papy::ast::Education *>(ast[1])->m_institution << "\n";
    std::cout << dynamic_cast<papy::ast::Education *>(ast[1])->m_degree << "\n";
    std::cout << dynamic_cast<papy::ast::Education *>(ast[1])->m_activities[0] << "\n";
    std::cout << dynamic_cast<papy::ast::Education *>(ast[1])->m_activities[1] << "\n";
    delete ast[1];
  }

  for (auto diag: parser.m_diagnostics) {
    std::cerr << diag.lineno << "|" << diag.message << "\n";
  }

  return 0;
}
