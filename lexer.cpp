//
// Created by aloussase on 7/11/25.
//

#include "lexer.h"

#include <cstring>
#include <iostream>
#include <cassert>

namespace papy {
   Lexer::Lexer(const char *s)
      : source(s) {
   }

   static char peek(Lexer *lexer) noexcept {
      if (lexer->current >= strlen(lexer->source)) return '\0';
      return lexer->source[lexer->current];
   }

   static char advance(Lexer *lexer) noexcept {
      if (lexer->current >= strlen(lexer->source)) return '\0';
      return lexer->source[lexer->current++];
   }

   static void emit_token(Lexer *lexer, TokenType type) noexcept {
      bool isString = type == TokenType::String;
      int x = isString ? 1 : 0;
      Token t;
      t.lexeme = std::string(&lexer->source[lexer->start] + x, lexer->current - lexer->start - x - x);
      t.lineno = lexer->lineno;
      t.pos = lexer->start;
      t.type = type;
      lexer->tokens.push_back(std::move(t));
   }

   [[nodiscard]]

   static bool string(char c, Lexer *lexer) noexcept {
      assert(c == '"');

      char k = peek(lexer);

      while (k && k != '\n' && k != '"') {
         advance(lexer);
         k = peek(lexer);
      }

      if (k != '"') {
         lexer->diagnostics.push_back({
            lexer->lineno,
            lexer->start,
            "Unterminated string literal"
         });
         return false;
      }

      advance(lexer);
      emit_token(lexer, TokenType::String);

      return true;
   }

   [[nodiscard]] static bool keyword(char c, Lexer *lexer) noexcept {
      char k = peek(lexer);

      while (k && k != ' ' && k != '\n') {
         advance(lexer);
         k = peek(lexer);
      }

      uint32_t n = lexer->current - lexer->start;

      if (strncmp(&lexer->source[lexer->start], "begin", n) == 0) {
         emit_token(lexer, TokenType::Begin);
      }
#define X(type, string) else if (strncmp(&lexer->source[lexer->start], string, n) == 0) emit_token(lexer, type);
      X(TokenType::End, "end")
      X(TokenType::Name, "name")
      X(TokenType::Email, "email")
      X(TokenType::PhoneNumber, "phone")
      X(TokenType::Tagline, "tagline")
      X(TokenType::Activity, "activity")
      X(TokenType::Institution, "institution")
      X(TokenType::Degree, "degree")
      X(TokenType::Company, "company")
      X(TokenType::StartDate, "start-date")
      X(TokenType::EndDate, "end-date")
      X(TokenType::Location, "location")
      X(TokenType::Title, "title")
      X(TokenType::TechStack, "stack")
      X(TokenType::Experience, "experience")
      X(TokenType::Metadata, "metadata")
      X(TokenType::Education, "education")
      X(TokenType::Activities, "activities")
      X(TokenType::Position, "position")
#undef X
      else {
         lexer->diagnostics.push_back({
            lexer->lineno,
            lexer->start,
            "Unrecognized keyword: '" + std::string(&lexer->source[lexer->start], n) + "'"
         });
         return false;
      }

      return true;
   }

   std::vector<Token> Lexer::lex() noexcept {
      char c = advance(this);
      while (c) {
         switch (c) {
            case '"':
               if (!string(c, this))
                  return {};
               break;
            case '\n':
               lineno += 1;
               break;
            case ' ':
               break;
            default:
               if (!keyword(c, this))
                  return {};
               break;
         }

         start = current;
         c = advance(this);
      }
      return tokens;
   }
}
