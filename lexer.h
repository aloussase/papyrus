#ifndef LEXER_H
#define LEXER_H

#include <cstdint>
#include <vector>

#include "diagnostic.h"
#include "token.h"

namespace papy {
    struct Lexer {
        uint32_t current = 0;
        uint32_t start = 0;
        uint32_t lineno = 1;
        const char *source;
        std::vector<Token> tokens;
        std::vector<Diagnostic> diagnostics;

        explicit Lexer(const char *);

        [[nodiscard]] std::vector<Token> lex() noexcept;
    };
}

#endif //LEXER_H
