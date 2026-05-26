#include <world/color.h>

#include <algorithm>
#include <cmath>

namespace renderer {

Color::Color(std::uint8_t r, std::uint8_t g, std::uint8_t b) : r_(r), g_(g), b_(b) {}

namespace {
std::uint8_t scale_channel(std::uint8_t channel, float intensity) {
    float scaled = static_cast<float>(channel) * intensity;
    scaled = std::clamp(scaled, 0.0f, 255.0f);
    return static_cast<std::uint8_t>(std::lround(scaled));
}
}  // namespace

Color operator*(const Color& color, float intensity) {
    return Color{scale_channel(color.r(), intensity), scale_channel(color.g(), intensity), scale_channel(color.b(), intensity)};
}

Color operator*(float intensity, const Color& color) {
    return color * intensity;
}

}  // namespace renderer
