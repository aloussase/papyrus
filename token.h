//
// Created by aloussase on 7/11/25.
//

#ifndef TOKEN_H
#define TOKEN_H

#include <cstdint>
#include <string>

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
}

#endif //TOKEN_H
