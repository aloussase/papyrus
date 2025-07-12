#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H

#include <stdint.h>
#include <string>

namespace papy {
    struct Diagnostic {
        uint32_t lineno;
        uint32_t pos;
        std::string message;
    };
}

#endif //DIAGNOSTIC_H
