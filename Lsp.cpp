//
// Created by aloussase on 7/13/25.
//

#include <cstdio>
#include <sstream>

#include <lsp/connection.h>
#include <lsp/io/standardio.h>
#include <lsp/messagehandler.h>
#include <lsp/messages.h>

#include "Parser.h"

#define LOGFILE stderr

namespace lspframework = ::lsp;

namespace papy::lspserver
{
[[nodiscard]] static std::string
readTextDocument(lspframework::TextDocumentIdentifier& textDocument)
{
    auto doc          = textDocument.uri.path();
    char buffer[4096] = { 0 };
    auto* fp          = fopen(doc.begin(), "r");
    fread(buffer, sizeof(char), sizeof(buffer) / sizeof(char), fp);
    fclose(fp);
    return std::string(buffer);
}

[[noreturn]] void start()
{
    auto connection     = lspframework::Connection(lspframework::io::standardIO());
    auto messageHandler = lspframework::MessageHandler(connection);

    messageHandler
        .add<lspframework::requests::Initialize>([](lspframework::requests::Initialize::Params&&) {
            lspframework::ServerCapabilities capabilities;

            capabilities.hoverProvider    = true;
            capabilities.textDocumentSync = lspframework::TextDocumentSyncKind::Full;

            return lspframework::requests::Initialize::Result {
                .capabilities = capabilities,
                .serverInfo
                = lspframework::InitializeResultServerInfo { .name    = "papy-language-server",
                                                             .version = "1.0.0" },
            };
        })
        .add<lspframework::requests::TextDocument_Hover>(
            [](lspframework::requests::TextDocument_Hover::Params&& params) {
                std::stringstream contents { readTextDocument(params.textDocument) };
                std::string line;
                unsigned int i = 0;

                while (i <= params.position.line)
                {
                    std::getline(contents, line);
                    i++;
                }

                unsigned int w = 0;
                unsigned int x = 0;
                while (x < params.position.character)
                {
                    if (line[x] == ' ')
                    {
                        w = x;
                    }
                    x++;
                }

                while (x < line.size() && line[x] != ' ' && line[x] != '\n')
                {
                    x++;
                }

                // Skip the space.
                w += 1;

                auto symbol = line.substr(w, x - w);
                fprintf(LOGFILE, "Got hover request for symbol: %s\n", symbol.c_str());

                std::string hover;

                if (symbol == "institution")
                {
                    hover = "The institution where you obtained this degree";
                }
                else if (symbol == "degree")
                {
                    hover = "The degree obtained";
                }
                else if (symbol == "metadata")
                {
                    hover = "Data about yourself such as your name and email";
                }
                else if (symbol == "education")
                {
                    hover = "Your education history";
                }
                else if (symbol == "experience")
                {
                    hover = "Your work experience";
                }

                return lspframework::requests::TextDocument_Hover::Result { lspframework::Hover {
                    hover } };
            })
        .add<lsp::notifications::TextDocument_DidSave>(
            [&messageHandler](lsp::notifications::TextDocument_DidSave::Params&& params) {
                std::string contents = readTextDocument(params.textDocument);

                papy::Parser parser { contents.c_str() };
                auto ast = parser.parse();

                std::for_each(ast.begin(), ast.end(), [](papy::ast::Node* node) { delete node; });

                auto& diagnostics = parser.m_diagnostics;
                auto response
                    = lspframework::notifications::TextDocument_PublishDiagnostics::Params {};

                response.uri = params.textDocument.uri;

                for (const auto& d : diagnostics)
                {
                    response.diagnostics.push_back(lspframework::Diagnostic{
                    .range =
                      lspframework::Range{
                          .start = { .line = d.lineno - 1, .character = d.pos },
                          .end = { .line = d.lineno - 1, .character = d.pos + 5 },
                      },
                  .message = d.message,
                  .severity = lspframework::DiagnosticSeverity::Error,
              });
                }

                messageHandler
                    .sendNotification<lspframework::notifications::TextDocument_PublishDiagnostics>(
                        std::move(response));
            });

    while (true)
        messageHandler.processIncomingMessages();
}
} // namespace papy::lspserver

auto main() -> int
{
    papy::lspserver::start();
}
