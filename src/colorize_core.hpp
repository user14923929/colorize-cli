// colorize_core.hpp — core color-resolution and text-colorizing logic,
// separated from the CLI so it can be unit tested independently.
#pragma once

#include <optional>
#include <string>

namespace colorize_core {

// layer must be "fg" or "bg". Throws std::invalid_argument on bad input.
std::string resolveColor(const std::string& value, const std::string& layer);

struct ColorizeOptions {
    std::optional<std::string> fg;
    std::optional<std::string> bg;
    bool bold = false;
    bool italic = false;
    bool underline = false;
};

// Wraps text in ANSI escape codes according to opts.
// Returns the text unchanged if no options are set.
std::string colorize(const std::string& text, const ColorizeOptions& opts);

}  // namespace colorize_core
