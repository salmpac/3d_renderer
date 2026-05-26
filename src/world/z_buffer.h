#pragma once

#include <geometry/constants.h>
#include <world/screen.h>

#include <vector>

namespace renderer {

class ZBuffer {
   public:
    ZBuffer(Width width, Height height);

    int width() const {
        return width_;
    }
    int height() const {
        return height_;
    }

    void clear();
    FloatType get(int x, int y) const;
    bool test_and_set(int x, int y, FloatType depth);  // returns true if pixel was updated

   private:
    int width_;
    int height_;
    std::vector<FloatType> depth_;
};

}  // namespace renderer
