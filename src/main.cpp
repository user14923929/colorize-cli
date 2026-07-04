// main.cpp — CLI entry point for colorize.
// Actual color logic lives in colorize_core.{hpp,cpp} so it stays unit-testable.

#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "colorize_core.hpp"

namespace {

void printHelp(const char* progName) {
    std::cout <<
        "Usage: " << progName << " <text> [options]\n\n"
        "Colorize text in the terminal (name / HEX / 256-index)\n\n"
        "Positional arguments:\n"
        "  text                  Text to colorize\n\n"
        "Options:\n"
        "  -c, --color <value>   Text color: red, #ff6b35, 202...\n"
        "  -b, --bg <value>      Background color: same format\n"
        "      --bold            Bold text\n"
        "      --italic          Italic text\n"
        "      --underline       Underlined text\n"
        "      --list-colors     Show the list of named colors and exit\n"
        "  -h, --help            Show this help message and exit\n";
}

void listColors() {
    static const std::vector<std::string> kNames = {
        "black", "red", "green", "yellow", "blue", "magenta", "cyan", "white",
        "bright_black", "bright_red", "bright_green", "bright_yellow",
        "bright_blue", "bright_magenta", "bright_cyan", "bright_white",
    };
    for (const auto& name : kNames) {
        colorize_core::ColorizeOptions opts;
        opts.fg = name;
        std::cout << colorize_core::colorize("\u25A0 " + name, opts) << "\n";
    }
}

}  // namespace

int main(int argc, char** argv) {
    std::vector<std::string> args(argv + 1, argv + argc);

    if (args.empty()) {
        printHelp(argv[0]);
        return 1;
    }

    colorize_core::ColorizeOptions opts;
    std::optional<std::string> text;

    for (size_t i = 0; i < args.size(); ++i) {
        const std::string& arg = args[i];

        auto nextValue = [&](const std::string& flag) -> std::string {
            if (i + 1 >= args.size()) {
                std::cerr << "Error: " << flag << " requires a value\n";
                std::exit(2);
            }
            return args[++i];
        };

        if (arg == "-h" || arg == "--help") {
            printHelp(argv[0]);
            return 0;
        } else if (arg == "--list-colors") {
            listColors();
            return 0;
        } else if (arg == "-c" || arg == "--color") {
            opts.fg = nextValue(arg);
        } else if (arg == "-b" || arg == "--bg") {
            opts.bg = nextValue(arg);
        } else if (arg == "--bold") {
            opts.bold = true;
        } else if (arg == "--italic") {
            opts.italic = true;
        } else if (arg == "--underline") {
            opts.underline = true;
        } else if (!arg.empty() && arg[0] == '-' && arg != "-") {
            std::cerr << "Error: unknown option '" << arg << "'\n";
            return 2;
        } else {
            if (text.has_value()) {
                std::cerr << "Error: multiple positional arguments given\n";
                return 2;
            }
            text = arg;
        }
    }

    if (!text.has_value()) {
        std::cerr << "Error: missing required argument 'text'\n";
        printHelp(argv[0]);
        return 2;
    }

    try {
        std::cout << colorize_core::colorize(*text, opts) << "\n";
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 2;
    }

    return 0;
}
