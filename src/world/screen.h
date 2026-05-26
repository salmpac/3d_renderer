#pragma once

#include <world/color.h>

#include <vector>

namespace renderer {

enum Width : int {};
enum Height : int {};

class Screen {
   public:
    Screen(Width width, Height height);

    int width() const {
        return width_;
    }
    int height() const {
        return height_;
    }

    void fill(Color color);
    void set(int x, int y, Color color);
    Color get(int x, int y) const;

    const Color* data() const {
        return pixels_.data();
    }

   private:
    int width_;
    int height_;
    std::vector<Color> pixels_;
};

}  // namespace renderer
