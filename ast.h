//
// Created by aloussase on 7/11/25.
//

#ifndef AST_H
#define AST_H
#include <string>
#include <vector>

namespace papy::ast {
    struct Node {
        virtual ~Node() {
        }
    };

    struct Metadata : public Node {
        std::string m_name;
        std::string m_email;
        std::string m_phone_number;
        std::string m_tagline;
    };

    struct Position {
        std::string m_company;
        std::string m_start_date;
        std::string m_end_date;
        std::string m_location;
        std::string m_title;
        std::string m_tech_stack;
        std::vector<std::string> m_activities;
    };

    struct Experience : public Node {
        std::vector<Position> m_positions;
    };

    struct Education : public Node {
        std::string m_institution;
        std::string m_degree;
        std::vector<std::string> m_activities;
    };
}


#endif //AST_H
