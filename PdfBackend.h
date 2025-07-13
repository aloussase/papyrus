//
// Created by aloussase on 7/12/25.
//

#ifndef PDFBACKEND_H
#define PDFBACKEND_H

#include "Backend.h"
#include <podofo/podofo.h>

namespace papy {
    class PdfBackend : public Backend {
    public:
        void accept(const std::vector<ast::Node *> &) override;

    private:
        [[nodiscard]] double draw(PoDoFo::PdfPainter &, ast::Metadata *) noexcept;

        [[nodiscard]] double draw(PoDoFo::PdfPainter &, ast::Education *) noexcept;

        [[nodiscard]] double draw(PoDoFo::PdfPainter &, ast::Experience *) noexcept;
    };
} // papy

#endif //PDFBACKEND_H
