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

begin experience
  begin position
    company "TIA Sa"
    start-date "01/01/1999"
    end-date "01/02/1999"
    location "Guayaquil"
    title "Software Developer"
    stack "Java, ReactNatvie"

    begin activities
      activity "Did some shit"
      activity "Did other shit"
    end
  end
end
)";

  papy::Parser parser(source);
  auto ast = parser.parse();

  auto had_error = !parser.m_diagnostics.empty();

  if (!had_error && ast.size() > 0 && ast[0]) {
    std::cout << dynamic_cast<papy::ast::Metadata *>(ast[0])->m_name << "\n";
    std::cout << dynamic_cast<papy::ast::Metadata *>(ast[0])->m_email << "\n";
    std::cout << dynamic_cast<papy::ast::Metadata *>(ast[0])->m_phone_number << "\n";
    std::cout << dynamic_cast<papy::ast::Metadata *>(ast[0])->m_tagline << "\n";
    delete ast[0];
  }

  if (!had_error && ast.size() >= 1 && ast[1]) {
    std::cout << dynamic_cast<papy::ast::Education *>(ast[1])->m_institution << "\n";
    std::cout << dynamic_cast<papy::ast::Education *>(ast[1])->m_degree << "\n";
    std::cout << dynamic_cast<papy::ast::Education *>(ast[1])->m_activities[0] << "\n";
    std::cout << dynamic_cast<papy::ast::Education *>(ast[1])->m_activities[1] << "\n";
    delete ast[1];
  }

  if (!had_error && ast.size() >= 2 && ast[2]) {
    std::cout << dynamic_cast<papy::ast::Experience *>(ast[2])->m_positions[0].m_company << "\n";
    std::cout << dynamic_cast<papy::ast::Experience *>(ast[2])->m_positions[0].m_start_date << "\n";
    std::cout << dynamic_cast<papy::ast::Experience *>(ast[2])->m_positions[0].m_end_date << "\n";
    std::cout << dynamic_cast<papy::ast::Experience *>(ast[2])->m_positions[0].m_location << "\n";
    std::cout << dynamic_cast<papy::ast::Experience *>(ast[2])->m_positions[0].m_title << "\n";
    std::cout << dynamic_cast<papy::ast::Experience *>(ast[2])->m_positions[0].m_tech_stack << "\n";
    std::cout << dynamic_cast<papy::ast::Experience *>(ast[2])->m_positions[0].m_activities[0] << "\n";
    std::cout << dynamic_cast<papy::ast::Experience *>(ast[2])->m_positions[0].m_activities[1] << "\n";
    delete ast[2];
  }

  for (auto diag: parser.m_diagnostics) {
    std::cerr << diag.lineno << "|" << diag.message << "\n";
  }

  return 0;
}
