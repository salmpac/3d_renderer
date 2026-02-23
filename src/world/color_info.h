#pragma once

#include <concepts>
#include <cstdint>

namespace renderer {

struct Color {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
};

template <typename T>
concept HasSetColorMethod = requires(T obj, int x, int y, Color color) {
    { obj.set(x, y, color) } -> std::same_as<void>;
};

}  // namespace renderer
