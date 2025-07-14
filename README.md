# 📜 Papy

Tiny DSL to compile your CV.

## Installation

### Install dependencies

```shell
sudo apt install -y libfontconfig1-dev libfreetype-dev libxml2-dev libssl-dev libjpeg-dev libpng-dev libtiff-dev
```

### Build

```shell
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Usage

Run the compiler providing a file name:

```shell
./build/papy cv.papy
```

Run in watch mode to refresh PDF on file changes:

```
./build/papy --watch cv.papy
```

## License
MIT
