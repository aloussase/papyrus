//
// Created by aloussase on 7/12/25.
//

#include "PdfBackend.h"

#include <podofo/podofo.h>

static double SX = 56.69;
static double SY;

static PoDoFo::PdfFont* normal_font;
static PoDoFo::PdfFont* bold_font;

static PoDoFo::PdfPage* document_page;

static double next_start;

constexpr int title_size      = 16;
constexpr int subtitle_size   = 12;
constexpr int mini_title_size = 11;
constexpr int body_size       = 10;

namespace papy
{
void use_bold_title(PoDoFo::PdfPainter& painter)
{
    painter.TextState.SetFont(*bold_font, title_size);
}

void use_mini_bold_title(PoDoFo::PdfPainter& painter)
{
    painter.TextState.SetFont(*bold_font, mini_title_size);
}

void use_subtitle(PoDoFo::PdfPainter& painter)
{
    painter.TextState.SetFont(*bold_font, subtitle_size);
}

void use_normal_body(PoDoFo::PdfPainter& painter)
{
    painter.TextState.SetFont(*normal_font, body_size);
}

double PdfBackend::draw(PoDoFo::PdfPainter& painter, ast::Metadata* meta) noexcept
{
    use_bold_title(painter);
    painter.DrawText(meta->m_name, SX, SY - 20);

    use_normal_body(painter);

    painter.DrawText(
        meta->m_tagline,
        document_page->GetRect().GetRight() - SX
            - normal_font->GetStringLength(meta->m_tagline, painter.TextState),
        SY - 20);

    auto contact = meta->m_email + " | " + meta->m_phone_number;
    painter.DrawText(
        contact,
        document_page->GetRect().GetRight() - SX
            - normal_font->GetStringLength(contact, painter.TextState),
        SY);

    painter.DrawLine(SX, SY - 20 - 8, document_page->GetRect().GetRight() - SX, SY - 20 - 8);

    return SY - 20 - title_size;
}

static std::vector<std::string> get_lines(const std::string& s) noexcept
{
    std::vector<std::string> lines;
    int words = 0;
    for (auto c : s)
    {
        if (c == ' ') words += 1;
    }

    if (words < 13)
    {
        lines.push_back(s);
        return lines;
    }

    std::string line;
    int x = 1;
    for (auto c : s)
    {
        if (c == ' ') x += 1;
        line += c;
        if (x % 13 == 0)
        {
            lines.push_back(line);
            line.clear();
            line += ' ';
            x = 1;
        }
    }

    lines.push_back(line);
    return lines;
}

double PdfBackend::draw(PoDoFo::PdfPainter& painter, ast::Education* ed) noexcept
{
    auto starty = next_start - 20;

    use_subtitle(painter);
    painter.DrawText("EDUCATION", SX, starty);
    use_normal_body(painter);

    painter.DrawLine(SX, starty - 8, document_page->GetRect().GetRight() - SX, starty - 8);

    starty -= 8;

    use_mini_bold_title(painter);
    painter.DrawText(ed->m_institution, SX, starty - 12);
    use_normal_body(painter);

    painter.DrawText(ed->m_degree, SX, starty - 25);

    starty -= 30;

    for (const auto& a : ed->m_activities)
    {
        auto text  = "- " + a;
        auto lines = get_lines(text);
        for (auto& line : lines)
        {
            painter.DrawText(line, SX, starty - 12);
            starty -= 12;
        }
    }

    return starty - 10;
}

double PdfBackend::draw(PoDoFo::PdfPainter& painter, ast::Experience* exp) noexcept
{
    auto starty = next_start - 20;

    use_subtitle(painter);
    painter.DrawText("EXPERIENCE", SX, starty);
    use_normal_body(painter);

    painter.DrawLine(SX, starty - 8, document_page->GetRect().GetRight() - SX, starty - 8);

    starty -= 8;

    for (const auto& position : exp->m_positions)
    {
        auto dates = position.m_start_date + " - " + position.m_end_date;

        use_mini_bold_title(painter);
        painter.DrawText(position.m_company, SX, starty - 15);
        use_normal_body(painter);

        painter.DrawText(position.m_title, SX, starty - 30);

        painter.DrawText(
            dates,
            document_page->GetRect().GetRight() - SX
                - normal_font->GetStringLength(dates, painter.TextState),
            starty - 15);

        painter.DrawText(
            position.m_location,
            document_page->GetRect().GetRight() - SX
                - normal_font->GetStringLength(position.m_location, painter.TextState),
            starty - 30);

        starty -= 35;

        for (const auto& activity : position.m_activities)
        {
            auto text = "- " + activity;
            painter.DrawText(text, SX, starty - 12);
            starty -= 12;
        }

        starty -= 10;
    }

    return starty;
}

void PdfBackend::accept(const std::vector<ast::Node*>& ast)
{
    PoDoFo::PdfStreamedDocument document("cv.pdf");
    PoDoFo::PdfPainter painter;

    auto& page = document.GetPages().CreatePage(PoDoFo::PdfPageSize::A4);
    painter.SetCanvas(page);

    // TODO: Make these configurable.
    normal_font = document.GetFonts().SearchFont("DejaVuSerif");
    bold_font   = document.GetFonts().SearchFont("DejaVuSerif-Bold");

    use_normal_body(painter);

    document_page = &page;
    SY            = page.GetRect().Height - 56.69;

    for (auto node : ast)
    {
        if (auto* meta = dynamic_cast<ast::Metadata*>(node))
        {
            next_start = draw(painter, meta);
        }
        else if (auto* exp = dynamic_cast<ast::Experience*>(node))
        {
            next_start = draw(painter, exp);
        }
        else if (auto* education = dynamic_cast<ast::Education*>(node))
        {
            next_start = draw(painter, education);
        }
        else
        {
            fprintf(stderr, "warning: tried to draw invalid node type\n");
        }
    }

    painter.FinishDrawing();
}
} // papy
