#pragma once

#include <geometry/constants.h>
#include <geometry/point3d.h>
#include <world/z_buffer.h>

#include <algorithm>
#include <cmath>

namespace renderer {

// Barycentric coordinates of a pixel inside a triangle (sum to 1).
struct Barycentric {
    FloatType w0;
    FloatType w1;
    FloatType w2;
};

class Rasterizer {
   public:
    template <typename ShadeFn>
    void rasterize(Point3D a, Point3D b, Point3D c, ZBuffer* zbuf, ShadeFn shade) const {
        FloatType area = edge(a, b, c);
        if (std::abs(area) < kEpsilon) {
            return;
        }

        int img_w = zbuf ? zbuf->width() : 0;
        int img_h = zbuf ? zbuf->height() : 0;

        int min_x = static_cast<int>(std::floor(std::min({a.x(), b.x(), c.x()})));
        int max_x = static_cast<int>(std::ceil(std::max({a.x(), b.x(), c.x()})));
        int min_y = static_cast<int>(std::floor(std::min({a.y(), b.y(), c.y()})));
        int max_y = static_cast<int>(std::ceil(std::max({a.y(), b.y(), c.y()})));

        if (zbuf) {
            min_x = std::max(0, min_x);
            min_y = std::max(0, min_y);
            max_x = std::min(img_w - 1, max_x);
            max_y = std::min(img_h - 1, max_y);
        }

        FloatType inv_area = FloatType{1} / area;

        for (int y = min_y; y <= max_y; ++y) {
            for (int x = min_x; x <= max_x; ++x) {
                Point3D pixel{static_cast<FloatType>(x) + FloatType{0.5}, static_cast<FloatType>(y) + FloatType{0.5}, 0};
                FloatType w0 = edge(b, c, pixel);
                FloatType w1 = edge(c, a, pixel);
                FloatType w2 = edge(a, b, pixel);
                if ((w0 < 0) || (w1 < 0) || (w2 < 0)) {
                    if (area > 0) {
                        continue;
                    }
                }
                if ((w0 > 0) || (w1 > 0) || (w2 > 0)) {
                    if (area < 0) {
                        continue;
                    }
                }
                w0 *= inv_area;
                w1 *= inv_area;
                w2 *= inv_area;
                FloatType z = w0 * a.z() + w1 * b.z() + w2 * c.z();
                if (z < 0 || z > 1) {
                    continue;
                }
                if (zbuf && !zbuf->test_and_set(x, y, z)) {
                    continue;
                }
                shade(x, y, Barycentric{w0, w1, w2});
            }
        }
    }

   private:
    static FloatType edge(Point3D a, Point3D b, Point3D c) {
        return (c.x() - a.x()) * (b.y() - a.y()) - (c.y() - a.y()) * (b.x() - a.x());
    }
};

}  // namespace renderer
