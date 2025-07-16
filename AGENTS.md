# AGENTS.md - Papyrus CV Compiler

## Build Commands
- **Build**: `cmake -B build -S . -DCMAKE_BUILD_TYPE=Release && cmake --build build`
- **Run**: `./build/papy cv.papy` or `./build/papy --watch cv.papy`
- **LSP Server**: `./build/papy-lsp-server`

## Testing
- Tests are in vendor/podofo/test (external library)
- No native test framework - verify manually with sample .papy files

## Code Style (.clang-format based)
- **Standard**: C++20
- **Style**: WebKit-based with custom braces
- **Pointer alignment**: Left (`char* ptr`)
- **Indentation**: 4 spaces
- **Column limit**: 100 characters
- **Braces**: Always break after control statements, functions, classes
- **Namespaces**: Use `papy` namespace for all project code

## Naming Conventions
- **Files**: PascalCase for headers/classes (`Parser.h`), snake_case for others (`main.cpp`)
- **Classes/Structs**: PascalCase (`Parser`, `Lexer`)
- **Variables**: snake_case (`m_source`, `current`, `lineno`)
- **Functions**: camelCase (`parse()`, `lex()`)
- **Member variables**: Prefix with `m_` (`m_diagnostics`)

## Error Handling
- Use `[[nodiscard]]` for important return values
- Store diagnostics in vectors (`std::vector<Diagnostic>`)
- Return error codes from main functions (0 = success, 1 = error)