// Minimal dependency-free test runner for colorize_core.
// No external test framework needed — keeps CI fast and simple.

#include <cassert>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>

#include "colorize_core.hpp"

using colorize_core::ColorizeOptions;
using colorize_core::colorize;
using colorize_core::resolveColor;

namespace {

int g_failures = 0;

void expectEq(const std::string& actual, const std::string& expected,
              const std::string& caseName) {
    if (actual != expected) {
        std::cerr << "[FAIL] " << caseName << "\n"
                   << "  expected: " << expected << "\n"
                   << "  actual:   " << actual << "\n";
        ++g_failures;
    } else {
        std::cout << "[ OK ] " << caseName << "\n";
    }
}

void expectThrows(const std::function<void()>& fn, const std::string& caseName) {
    try {
        fn();
        std::cerr << "[FAIL] " << caseName << " (expected exception, none thrown)\n";
        ++g_failures;
    } catch (const std::invalid_argument&) {
        std::cout << "[ OK ] " << caseName << "\n";
    }
}

}  // namespace

int main() {
    // Named color resolution
    expectEq(resolveColor("red", "fg"), "38;5;1", "named color fg (red)");
    expectEq(resolveColor("RED", "fg"), "38;5;1", "named color is case-insensitive");
    expectEq(resolveColor("blue", "bg"), "48;5;4", "named color bg (blue)");

    // HEX resolution
    expectEq(resolveColor("#ff6b35", "fg"), "38;2;255;107;53", "hex with hash (fg)");
    expectEq(resolveColor("ff6b35", "fg"), "38;2;255;107;53", "hex without hash (fg)");
    expectEq(resolveColor("#000000", "bg"), "48;2;0;0;0", "hex black (bg)");
    expectEq(resolveColor("#FFFFFF", "fg"), "38;2;255;255;255", "hex uppercase");

    // Index resolution
    expectEq(resolveColor("202", "fg"), "38;5;202", "256-index fg");
    expectEq(resolveColor("0", "bg"), "48;5;0", "256-index lower bound");
    expectEq(resolveColor("255", "fg"), "38;5;255", "256-index upper bound");

    // Invalid input
    expectThrows([] { resolveColor("not_a_color", "fg"); }, "invalid name throws");
    expectThrows([] { resolveColor("#fff", "fg"); }, "short hex throws");
    expectThrows([] { resolveColor("256", "fg"); }, "out-of-range index throws");
    expectThrows([] { resolveColor("-1", "fg"); }, "negative index throws");

    // colorize() composition
    ColorizeOptions opts;
    expectEq(colorize("hi", opts), "hi", "no options returns plain text");

    opts.bold = true;
    expectEq(colorize("hi", opts), "\033[1mhi\033[0m", "bold only");

    ColorizeOptions fgOnly;
    fgOnly.fg = "green";
    expectEq(colorize("hi", fgOnly), "\033[38;5;2mhi\033[0m", "fg only");

    ColorizeOptions fgBg;
    fgBg.fg = "#ffffff";
    fgBg.bg = "red";
    fgBg.bold = true;
    expectEq(colorize("hi", fgBg), "\033[1;38;2;255;255;255;48;5;1mhi\033[0m",
              "bold + fg hex + bg named");

    if (g_failures > 0) {
        std::cerr << "\n" << g_failures << " test(s) failed.\n";
        return 1;
    }
    std::cout << "\nAll tests passed.\n";
    return 0;
}
