#include "Parser.h"
#include <iostream>
#include <memory>

#include "PdfBackend.h"

// OPTIONS
static char *filename;

auto main(int argc, char **argv) -> int {
  argc--;
  argv++;

  if (argc != 1) {
    fprintf(stderr, "Missing filename argument\n");
    return 1;
  }

  filename = *argv;

  // TODO: Calculate file size in bytes.
  char source[4096] = {0};
  FILE *fp = fopen(filename, "r");
  fread(source, sizeof(char), sizeof(source) / sizeof(char), fp);

  papy::Parser parser(source);
  auto ast = parser.parse();

  // TODO: Make the backend configurable
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
