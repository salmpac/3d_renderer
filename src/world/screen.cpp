#include <world/screen.h>

#include <algorithm>

namespace renderer {

Screen::Screen(Width width, Height height)
    : width_(static_cast<int>(width)), height_(static_cast<int>(height)), pixels_(static_cast<std::size_t>(width_) * height_) {}

void Screen::fill(Color color) {
    std::fill(pixels_.begin(), pixels_.end(), color);
}

void Screen::set(int x, int y, Color color) {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        return;
    }
    pixels_[static_cast<std::size_t>(y) * width_ + x] = color;
}

Color Screen::get(int x, int y) const {
    return pixels_[static_cast<std::size_t>(y) * width_ + x];
}

}  // namespace renderer
