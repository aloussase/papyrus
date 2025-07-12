//
// Created by aloussase on 7/11/25.
//

#include "Parser.h"
#include "lexer.h"

#include <iostream>
#include <optional>

namespace papy {
    Parser::Parser(const char *s) noexcept: m_source(s) {
    }

    static void emit_diagnostic(Parser *parser, const std::string &message) noexcept {
        assert(parser->current - 1 < parser->m_tokens.size());
        auto current_token = parser->m_tokens[parser->current - 1];
        parser->m_diagnostics.push_back({
            current_token.lineno,
            current_token.pos,
            message
        });
    }

    [[nodiscard]] static std::optional<Token> advance(Parser *parser) {
        if (parser->current < parser->m_tokens.size())
            return parser->m_tokens[parser->current++];
        return {};
    }

    [[nodiscard]] static std::optional<Token> peek(Parser *parser) {
        if (parser->current < parser->m_tokens.size())
            return parser->m_tokens[parser->current];
        return {};
    }

    [[nodiscard]] static bool expect(Parser *parser, TokenType type) noexcept {
        if (!(parser->current - 1 < parser->m_tokens.size())) {
            emit_diagnostic(
                parser,
                "Expected '" + token::to_string(type) + "', but reached end of input");
            return false;
        }

        if (auto o_token = parser->m_tokens[parser->current - 1]; o_token.type == type) {
            return true;
        } else {
            emit_diagnostic(
                parser,
                "Expected '" + token::to_string(type) + "', but got '" + token::to_string(o_token.type) + "'");
        }

        return false;
    }

#define REQUIRE_TOKEN(P) \
    auto __o_token__ = advance(parser); \
    if (!__o_token__) return nullptr; \
    const auto &token = __o_token__.value();

#define EXPECT_STRING(P, R) \
    ({ \
        auto __os__ = expect_string(P);\
        if (!__os__) return (R);\
        __os__.value();\
    })

#define SET_STRING(node, type, field, R) \
    case type: \
        (void) advance(parser); \
        node->field = EXPECT_STRING(parser, R); \
        otoken = peek(parser);\
        break;

    [[nodiscard]] static std::optional<std::string> expect_string(Parser *parser) {
        if (auto otoken = advance(parser); otoken && otoken->type == TokenType::String) {
            return otoken->lexeme;
        } else if (otoken) {
            emit_diagnostic(
                parser,
                "Expected a string but got " + token::to_string(otoken->type));
        } else {
            emit_diagnostic(
                parser,
                "Unexpected end of input");
        }
        return {};
    }

    [[nodiscard]] std::optional<std::string> activity(Parser *parser) noexcept {
        if (auto ot = peek(parser); ot && ot->type == TokenType::Activity) {
            (void) advance(parser);
            return expect_string(parser);
        }
        return {};
    }

    [[nodiscard]] bool activities(Parser *parser, std::vector<std::string> &as) noexcept {
        auto otoken = peek(parser);

        if (!otoken && otoken->type != TokenType::Begin) return true;
        (void) advance(parser);

        (void) advance(parser);
        if (!expect(parser, TokenType::Activities)) return false;

        auto act = activity(parser);
        while (act) {
            as.push_back(std::move(act.value()));
            act = activity(parser);
        }

        (void) advance(parser);
        if (!expect(parser, TokenType::End)) return false;

        return true;
    }

    [[nodiscard]] static ast::Node *metadata(Parser *parser) {
        auto otoken = peek(parser);
        auto *node = new ast::Metadata;
        while (otoken) {
            switch (otoken->type) {
                SET_STRING(node, TokenType::Name, m_name, nullptr);
                SET_STRING(node, TokenType::Email, m_email, nullptr);
                SET_STRING(node, TokenType::PhoneNumber, m_phone_number, nullptr);
                SET_STRING(node, TokenType::Tagline, m_tagline, nullptr);
                case TokenType::End:
                    (void) advance(parser);
                    goto done;
                default:
                    emit_diagnostic(
                        parser,
                        "Invalid token in metadata block: " + token::to_string(otoken->type));
                    delete node;
                    return nullptr;
            }
        }
    done:
        if (!expect(parser, TokenType::End)) {
            delete node;
            return nullptr;
        }
        return node;
    }

    [[nodiscard]] static ast::Node *education(Parser *parser) {
        auto otoken = peek(parser);
        auto *node = new ast::Education;
        while (otoken) {
            switch (otoken->type) {
                SET_STRING(node, TokenType::Institution, m_institution, nullptr);
                SET_STRING(node, TokenType::Degree, m_degree, nullptr);
                case TokenType::Begin:
                    if (!activities(parser, node->m_activities))
                        return nullptr;
                    otoken = peek(parser);
                    break;
                case TokenType::End:
                    (void) advance(parser);
                    goto done;
                default:
                    emit_diagnostic(
                        parser,
                        "Invalid token in education block: " + token::to_string(otoken->type));
                    return nullptr;
            }
        }
    done:
        if (!expect(parser, TokenType::End))
            return nullptr;
        return node;
    }

    [[nodiscard]] static bool position(Parser *parser, ast::Position &position) {
        auto otoken = peek(parser);
        assert(otoken && "Expected 'begin' but got nothing");
        assert(otoken->type == TokenType::Begin && "Expected 'begin' but got something else");

        (void) advance(parser);
        (void) advance(parser);

        if (!expect(parser, TokenType::Position)) {
            return false;
        }

        while ((otoken = peek(parser))) {
            switch (otoken->type) {
                SET_STRING((&position), TokenType::Company, m_company, false);
                SET_STRING((&position), TokenType::StartDate, m_start_date, false);
                SET_STRING((&position), TokenType::EndDate, m_end_date, false);
                SET_STRING((&position), TokenType::Location, m_location, false);
                SET_STRING((&position), TokenType::Title, m_title, false);
                SET_STRING((&position), TokenType::TechStack, m_tech_stack, false);
                case TokenType::End:
                    (void) advance(parser);
                    goto done;
                case TokenType::Begin:
                    if (!activities(parser, position.m_activities))
                        return false;
                    break;
                default:
                    emit_diagnostic(parser,
                                    "Invalid token in position block: " + token::to_string(otoken->type));
                    return false;
            }
        }

    done:

        if (!expect(parser, TokenType::End)) return false;

        return true;
    }

    [[nodiscard]] static ast::Node *experience(Parser *parser) {
        assert(parser->m_tokens[parser->current - 1].type == TokenType::Experience);

        auto otoken = peek(parser);
        auto *node = new ast::Experience;

        while (otoken && otoken->type == TokenType::Begin) {
            ast::Position p;
            if (!position(parser, p)) {
                delete node;
                return nullptr;
            }
            node->m_positions.push_back(std::move(p));
            otoken = peek(parser);
        }

        (void) advance(parser);
        if (!expect(parser, TokenType::End)) {
            delete node;
            return nullptr;
        }

        return node;
    }

    [[nodiscard]] static ast::Node *block(Parser *parser) {
        REQUIRE_TOKEN(parser)
        switch (token.type) {
            case TokenType::Metadata:
                return metadata(parser);
            case TokenType::Experience:
                return experience(parser);
            case TokenType::Education:
                return education(parser);
            default:
                emit_diagnostic(
                    parser, "Invalid start of block, expected one of 'metadata', 'experience' or 'education'");
                return nullptr;
        }
    }

    [[nodiscard]] static ast::Node *do_parse(Parser *parser) {
        REQUIRE_TOKEN(parser)
        switch (token.type) {
            case TokenType::Begin:
                return block(parser);
            default:
                emit_diagnostic(
                    parser, "Invalid start of block, expected 'begin', but got: " + token::to_string(token.type));
                return nullptr;
        }
    }

#undef REQUIRE_TOKEN
#undef SET_STRING
#undef EXPECT_STRING

    [[nodiscard]] std::vector<ast::Node *> Parser::parse() noexcept {
        Lexer lexer(m_source);
        m_tokens = lexer.lex();

        if (m_tokens.empty()) {
            m_diagnostics = lexer.diagnostics;
            return {};
        }

        std::vector<ast::Node *> nodes;
        ast::Node *node;

        while ((node = do_parse(this)) != nullptr)
            nodes.push_back(node);

        return nodes;
    }
} // papy
