#include "Parser.h"
#include <iostream>
#include <memory>

#include "FileWatcher.h"
#include "PdfBackend.h"

// OPTIONS
static char *filename;
static bool watch = false;

[[nodiscard]] int run() {
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

auto main(int argc, char **argv) -> int {
  argc--;
  argv++;

  for (; argc > 0; argc--, argv++) {
    if (strcmp(*argv, "-w") == 0 || strcmp(*argv, "--watch") == 0) {
      watch = true;
    } else {
      break;
    }
  }

  if (argc != 1) {
    fprintf(stderr, "Missing filename argument\n");
    return 1;
  }

  filename = *argv;

  if (watch) {
    FileWatcher watcher{filename};
    watcher.watch(run).join();
  } else {
    return run();
  }
}
