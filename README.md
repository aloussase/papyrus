# 📜 Papyrus

A modern, lightweight domain-specific language (DSL) for creating beautiful CVs and resumes. Write your professional profile in a clean, structured format and compile it to polished PDF documents.

## 🚀 Features

- **Simple Syntax**: Intuitive DSL for describing your professional experience
- **PDF Output**: High-quality PDF generation using PoDoFo library
- **Watch Mode**: Auto-recompile on file changes for rapid development
- **Language Server**: Full LSP support with syntax highlighting and error checking
- **Structured Data**: Organize metadata, education, experience, and activities

## 📋 Project Description

Papyrus transforms structured text files (`.papy`) into professional PDF documents. The language provides clear blocks for different sections of your CV:

- `metadata`: Personal information (name, email, phone, tagline)
- `education`: Academic background and activities
- `experience`: Work history with positions, dates, and accomplishments

## 🔧 Installation

### Prerequisites

Install the required system dependencies:

```bash
# Ubuntu/Debian
sudo apt install -y libfontconfig1-dev libfreetype-dev libxml2-dev libssl-dev libjpeg-dev libpng-dev libtiff-dev

# Arch Linux
sudo pacman -S fontconfig freetype2 libxml2 openssl libjpeg-turbo libpng libtiff

# macOS (with Homebrew)
brew install fontconfig freetype libxml2 openssl jpeg libpng libtiff
```

### Build from Source

```bash
git clone https://github.com/aloussase/papyrus.git
cd papyrus
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## 🎯 Usage

### Basic Compilation

Compile a `.papy` file to PDF:

```bash
./build/papy cv.papy
```

### Watch Mode

Auto-recompile on file changes (perfect for development):

```bash
./build/papy --watch cv.papy
```

### Sample Execution

Here's what a typical workflow looks like:

```bash
$ ./build/papy sample.papy
✅ Successfully compiled sample.papy → sample.pdf

$ ./build/papy --watch cv.papy
🔄 Watching cv.papy for changes...
✅ Compiled cv.papy → cv.pdf
🔄 File changed, recompiling...
✅ Compiled cv.papy → cv.pdf
```

### Example `.papy` File

```papy
begin metadata
  name "John Doe"
  email "john.doe@gmail.com"
  phone "XXXXX"
  tagline "Software Developer"
end

begin education
  institution "Some school"
  degree "Bsc. Computer Science"
  begin activities
    activity "President of Open Source club"
    activity "Runner-up for district's attorney"
  end
end

begin experience
  begin position
    company "Acme Inc"
    start-date "January 2025"
    end-date ""
    location "Remote"
    title "Software Developer"
    stack "Java"

    begin activities
      activity "Developed distributed applications"
      activity "Built microfrontend components"
    end
  end
end
```

## 🔧 Language Server Protocol (LSP)

Papyrus includes a full-featured LSP server for enhanced development experience.

### Building the LSP Server

The LSP server is built automatically with the main project:

```bash
./build/papy-lsp-server
```

### Neovim Configuration

Add this configuration to your Neovim setup:

```lua
-- In your init.lua or appropriate config file
local lspconfig = require('lspconfig')

-- Add papy LSP configuration
local configs = require('lspconfig.configs')

if not configs.papy then
  configs.papy = {
    default_config = {
      cmd = { '/path/to/papyrus/build/papy-lsp-server' },
      filetypes = { 'papy' },
      root_dir = lspconfig.util.root_pattern('.git', '.'),
      settings = {},
    },
  }
end

lspconfig.papy.setup({
  on_attach = function(client, bufnr)
    -- Your preferred LSP keybindings here
    local opts = { buffer = bufnr }
    vim.keymap.set('n', 'gd', vim.lsp.buf.definition, opts)
    vim.keymap.set('n', 'K', vim.lsp.buf.hover, opts)
    vim.keymap.set('n', '<leader>rn', vim.lsp.buf.rename, opts)
  end,
  capabilities = require('cmp_nvim_lsp').default_capabilities(),
})

-- File type detection
vim.filetype.add({
  extension = {
    papy = 'papy',
  },
})
```

### LSP Features

- ✅ **Error Diagnostics**: Real-time error checking and reporting
- ✅ **Hover Information**: Contextual help and documentation

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request. For major changes, please open an issue first to discuss what you would like to change.

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
