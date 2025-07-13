//
// Created by aloussase on 7/12/25.
//

#ifndef BACKEND_H
#define BACKEND_H

#include "ast.h"

namespace papy {
    class Backend {
    public:
        virtual ~Backend() = default;

        virtual void accept(const std::vector<ast::Node *> &) = 0;
    };
}

#endif //BACKEND_H
