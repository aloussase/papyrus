//
// Created by aloussase on 7/11/25.
//

#ifndef TOKEN_H
#define TOKEN_H

#include <cstdint>
#include <string>
#include <cassert>

namespace papy {
    enum class TokenType {
        String,
        // Blocks
        Begin,
        End,
        // Metadata
        Name,
        Email,
        PhoneNumber,
        Tagline,
        // All
        Activity,
        // Education
        Institution,
        Degree,
        // Experience
        Company,
        StartDate,
        EndDate,
        Location,
        Title,
        TechStack,
        // Sections
        Experience,
        Metadata,
        Education,
        Activities,
    };

    struct Token {
        uint32_t pos;
        uint32_t lineno;
        TokenType type;
        std::string lexeme;
    };

    namespace token {
        [[nodiscard]] static std::string to_string(TokenType type) noexcept {
            switch (type) {
                case TokenType::String: return "string";
                case TokenType::Begin: return "begin";
                case TokenType::End: return "end";
                case TokenType::Name: return "name";
                case TokenType::Email: return "email";
                case TokenType::PhoneNumber: return "phone number";
                case TokenType::Tagline: return "tagline";
                case TokenType::Activity: return "activity";
                case TokenType::Institution: return "institution";
                case TokenType::Degree: return "degree";
                case TokenType::Company: return "company";
                case TokenType::StartDate: return "start date";
                case TokenType::EndDate: return "end date";
                case TokenType::Location: return "location";
                case TokenType::Title: return "title";
                case TokenType::TechStack: return "tech stack";
                case TokenType::Experience: return "experience";
                case TokenType::Metadata: return "metadata";
                case TokenType::Education: return "education";
                case TokenType::Activities: return "activities";
                default: assert(0 && "Unexpected token type");
            }
        }
    }
}

#endif //TOKEN_H
