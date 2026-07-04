#include "colorize_core.hpp"

#include <algorithm>
#include <cctype>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace colorize_core {
namespace {

const std::unordered_map<std::string, int> kNamedColors = {
    {"black", 0},          {"red", 1},           {"green", 2},
    {"yellow", 3},         {"blue", 4},          {"magenta", 5},
    {"cyan", 6},           {"white", 7},         {"bright_black", 8},
    {"bright_red", 9},     {"bright_green", 10}, {"bright_yellow", 11},
    {"bright_blue", 12},   {"bright_magenta", 13}, {"bright_cyan", 14},
    {"bright_white", 15},
};

const std::regex kHexRe("^#?([0-9a-fA-F]{6})$");

std::string toLower(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return out;
}

bool isAllDigits(const std::string& s) {
    if (s.empty()) return false;
    return std::all_of(s.begin(), s.end(), [](unsigned char c) {
        return std::isdigit(c) != 0;
    });
}

}  // namespace

std::string resolveColor(const std::string& value, const std::string& layer) {
    const int base = (layer == "fg") ? 38 : 48;

    // 1. Named color
    const std::string lowered = toLower(value);
    auto it = kNamedColors.find(lowered);
    if (it != kNamedColors.end()) {
        return std::to_string(base) + ";5;" + std::to_string(it->second);
    }

    // 2. HEX (#ff6b35 or ff6b35)
    std::smatch match;
    if (std::regex_match(value, match, kHexRe)) {
        const std::string hexPart = match[1].str();
        const int r = std::stoi(hexPart.substr(0, 2), nullptr, 16);
        const int g = std::stoi(hexPart.substr(2, 2), nullptr, 16);
        const int b = std::stoi(hexPart.substr(4, 2), nullptr, 16);
        return std::to_string(base) + ";2;" + std::to_string(r) + ";" +
               std::to_string(g) + ";" + std::to_string(b);
    }

    // 3. Numeric index 0-255
    if (isAllDigits(value)) {
        const int idx = std::stoi(value);
        if (idx >= 0 && idx <= 255) {
            return std::to_string(base) + ";5;" + value;
        }
    }

    throw std::invalid_argument(
        "Cannot parse color '" + value +
        "'. Use a name (red, cyan...), a HEX code (#ff6b35), or an index (0-255).");
}

std::string colorize(const std::string& text, const ColorizeOptions& opts) {
    std::vector<std::string> codes;
    if (opts.bold) codes.push_back("1");
    if (opts.italic) codes.push_back("3");
    if (opts.underline) codes.push_back("4");
    if (opts.fg) codes.push_back(resolveColor(*opts.fg, "fg"));
    if (opts.bg) codes.push_back(resolveColor(*opts.bg, "bg"));

    if (codes.empty()) return text;

    std::ostringstream oss;
    oss << "\033[";
    for (size_t i = 0; i < codes.size(); ++i) {
        oss << codes[i];
        if (i + 1 < codes.size()) oss << ";";
    }
    oss << "m" << text << "\033[0m";
    return oss.str();
}

}  // namespace colorize_core
