#pragma once

#include <cstdint>

namespace renderer {

class Color {
   public:
    Color() = default;
    Color(std::uint8_t r, std::uint8_t g, std::uint8_t b);

    std::uint8_t r() const {
        return r_;
    }
    std::uint8_t g() const {
        return g_;
    }
    std::uint8_t b() const {
        return b_;
    }

   private:
    std::uint8_t r_ = 0;
    std::uint8_t g_ = 0;
    std::uint8_t b_ = 0;
};

Color operator*(const Color& color, float intensity);
Color operator*(float intensity, const Color& color);

}  // namespace renderer
