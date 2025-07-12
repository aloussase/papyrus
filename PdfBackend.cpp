//
// Created by aloussase on 7/12/25.
//

#include "PdfBackend.h"

#include <podofo/podofo.h>


static double SX = 56.69;
static double SY;

static PoDoFo::PdfFont *normal_font;
static PoDoFo::PdfFont *bold_font;

static PoDoFo::PdfPage *document_page;

constexpr int title_size = 16;
constexpr int body_size = 10;

namespace papy {
    void use_bold_title(PoDoFo::PdfPainter &painter) {
        painter.TextState.SetFont(*bold_font, title_size);
    }

    void use_normal_body(PoDoFo::PdfPainter &painter) {
        painter.TextState.SetFont(*normal_font, body_size);
    }

    void PdfBackend::draw(PoDoFo::PdfPainter &painter, ast::Metadata *meta) noexcept {
        use_bold_title(painter);
        painter.DrawText(meta->m_name, SX, SY - 20);

        use_normal_body(painter);

        painter.DrawText(
            meta->m_tagline,
            document_page->GetRect().GetRight() - SX - normal_font->GetStringLength(meta->m_tagline, painter.TextState),
            SY - 20);

        auto contact = meta->m_email + " | " + meta->m_phone_number;
        painter.DrawText(
            contact,
            document_page->GetRect().GetRight() - SX - normal_font->GetStringLength(contact, painter.TextState),
            SY);

        painter.DrawLine(
            SX,
            SY - 20 - title_size,
            document_page->GetRect().GetRight() - SX,
            SY - 20 - title_size);
    }

    void PdfBackend::draw(PoDoFo::PdfPainter &, ast::Education *) noexcept {
    }

    void PdfBackend::draw(PoDoFo::PdfPainter &, ast::Experience *) noexcept {
    }

    void PdfBackend::accept(const std::vector<ast::Node *> &ast) {
        PoDoFo::PdfStreamedDocument document("cv.pdf");
        PoDoFo::PdfPainter painter;

        auto &page = document.GetPages().CreatePage(PoDoFo::PdfPageSize::A4);
        painter.SetCanvas(page);

        normal_font = document.GetFonts().SearchFont("DejaVuSerif");
        bold_font = document.GetFonts().SearchFont("DejaVuSerif-Bold");

        use_normal_body(painter);

        document_page = &page;
        SY = page.GetRect().Height - 56.69;

        for (auto node: ast) {
            if (auto *meta = dynamic_cast<ast::Metadata *>(node)) {
                draw(painter, meta);
            } else if (auto *exp = dynamic_cast<ast::Experience *>(node)) {
                draw(painter, exp);
            } else if (auto *education = dynamic_cast<ast::Education *>(node)) {
                draw(painter, education);
            } else {
                fprintf(stderr, "warning: tried to draw invalid node type\n");
            }
        }

        painter.FinishDrawing();
    }
} // papy
