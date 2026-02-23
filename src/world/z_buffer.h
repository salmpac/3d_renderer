#pragma once

#include <geometry/primitives.h>

#include <limits>
#include <vector>

namespace renderer {

class ZBuffer {
   public:
    template <typename T>
    using PixelStore = std::vector<T>;

    struct PixelInfo {
        static constexpr size_t no_polygon = std::numeric_limits<size_t>::max();
        FloatType distance = FloatMax;
        size_t polygon_id = no_polygon;
        auto operator<=>(const PixelInfo&) const = default;
    };

   public:
    ZBuffer(int height, int width) : height(height), width(width), z_buffer(height * width) {}

    // polygon x and y should represent pixel x and y and z is depth
    // polygon.homogenize() should be called before add_polygon
    void add_polygon(const Polygon& polygon, size_t id);
    void clear();

    inline PixelInfo get_pixel_info(int x, int y) const {
        // if (x < 0 || x >= width || y < 0 || y >= height) {
        //     return {FloatMax, std::numeric_limits<size_t>::max()};
        // }
        return z_buffer[y * width + x];
    }

    inline int get_width() const { return width; }

    inline int get_height() const { return height; }

   protected:
    int height;
    int width;
    PixelStore<PixelInfo> z_buffer;
};

}  // namespace renderer
