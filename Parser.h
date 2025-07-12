//
// Created by aloussase on 7/11/25.
//

#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "diagnostic.h"
#include "token.h"

#include <cstdint>

namespace papy {
    struct Parser {
        const char *m_source;
        uint32_t current = 0;
        std::vector<Diagnostic> m_diagnostics = {};
        std::vector<Token> m_tokens = {};

        explicit Parser(const char *) noexcept;

        [[nodiscard]] std::vector<ast::Node *> parse() noexcept;
    };
} // papy

#endif //PARSER_H
