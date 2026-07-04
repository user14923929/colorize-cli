# colorize

A small, dependency-free CLI utility that colorizes terminal text using
named colors, HEX codes, or 256-color palette indexes.

```
$ colorize "Hello, world!" -c "#ff6b35" --bold
```

## Features

- **Named colors** — `red`, `green`, `bright_yellow`, etc. (16 base colors)
- **HEX truecolor** — `#ff6b35` or `ff6b35` (24-bit RGB, `\033[38;2;r;g;bm`)
- **256-color index** — any value `0-255` (`\033[38;5;Nm`)
- **Foreground and background** — `-c` / `-b`
- **Text styles** — `--bold`, `--italic`, `--underline`
- Zero runtime dependencies — pure C++17, no external libraries
- Ships with unit tests and CI (GCC + Clang, plus static analysis)

## Installation

### From the AUR

```bash
# Stable release
paru -S colorize
# or
yay -S colorize

# Latest git HEAD
paru -S colorize-git
```

### Build from source

Requires a C++17 compiler and CMake ≥ 3.16.

```bash
git clone https://github.com/user14923929/colorize-cli.git
cd colorize-cli
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
sudo cmake --install build
```

## Usage

```
Usage: colorize <text> [options]

Colorize text in the terminal (name / HEX / 256-index)

Positional arguments:
  text                  Text to colorize

Options:
  -c, --color <value>   Text color: red, #ff6b35, 202...
  -b, --bg <value>      Background color: same format
      --bold            Bold text
      --italic          Italic text
      --underline       Underlined text
      --list-colors     Show the list of named colors and exit
  -h, --help            Show this help message and exit
```

### Examples

```bash
# Named color
colorize "Error!" -c red --bold

# HEX color
colorize "Custom shade" -c "#ff6b35"

# HEX foreground + named background
colorize "Warning" -c "#ffffff" -b red --bold

# 256-color index
colorize "Index 202" -c 202

# List all named colors
colorize --list-colors
```

Any value that isn't a recognized name, a valid `#RRGGBB` hex code, or an
integer `0-255` is rejected with a clear error message and a non-zero exit
code (`2`), so it's safe to use in scripts.

## Why C++?

This started as a quick Python script and grew into a proper system-level
CLI utility — no runtime dependency on an interpreter, a single static
binary, and fast enough to spam thousands of times in a shell loop without
noticing the overhead.

## Project layout

```
colorize-cli/
├── src/
│   ├── colorize_core.hpp   # public API: resolveColor(), colorize()
│   ├── colorize_core.cpp   # color-resolution logic (unit tested)
│   └── main.cpp            # CLI argument parsing / entry point
├── tests/
│   └── test_colorize_core.cpp
├── CMakeLists.txt
├── PKGBUILD                # AUR package (stable release)
├── PKGBUILD-git             # AUR package (git HEAD)
└── .github/workflows/ci.yml
```

## Development

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

Static analysis:

```bash
cppcheck --enable=warning,style,performance,portability \
    --error-exitcode=1 --std=c++17 --inline-suppr src/
```

## Publishing to the AUR

```bash
makepkg --printsrcinfo > .SRCINFO
git add PKGBUILD .SRCINFO
git commit -m "Update to <version>"
git push aur master
```

## License

MIT — see [LICENSE](LICENSE).
