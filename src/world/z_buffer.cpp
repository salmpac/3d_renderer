#include <world/z_buffer.h>

#include <algorithm>
#include <cmath>

namespace renderer {

static inline FloatType edge_function(const Point4D& a, const Point4D& b, const Point4D& c) {
    return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]);
}

void ZBuffer::add_polygon(const Polygon& polygon, size_t id) {
    const Point4D& p0 = polygon.A;
    const Point4D& p1 = polygon.B;
    const Point4D& p2 = polygon.C;

    int min_x = std::max(0, (int)std::floor(std::min({p0[0], p1[0], p2[0]})));
    int max_x = std::min(width - 1, (int)std::ceil(std::max({p0[0], p1[0], p2[0]})));
    int min_y = std::max(0, (int)std::floor(std::min({p0[1], p1[1], p2[1]})));
    int max_y = std::min(height - 1, (int)std::ceil(std::max({p0[1], p1[1], p2[1]})));

    FloatType area = edge_function(p0, p1, p2);

    if (std::abs(area) < EPSILON) {
        return;
    }

    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            Point4D pixel_pt((FloatType)x + 0.5f, (FloatType)y + 0.5f, 0);
            FloatType w0 = edge_function(p1, p2, pixel_pt);
            FloatType w1 = edge_function(p2, p0, pixel_pt);
            FloatType w2 = edge_function(p0, p1, pixel_pt);

            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                w0 /= area;
                w1 /= area;
                w2 /= area;
                FloatType z = w0 * p0[2] + w1 * p1[2] + w2 * p2[2];

                if (z < 0.0f || z > 1.0f) {
                    continue;
                }

                int idx = y * width + x;
                if (z < z_buffer[idx].distance) {
                    z_buffer[idx].distance = z;
                    z_buffer[idx].polygon_id = id;
                }
            }
        }
    }
}

void ZBuffer::clear() {
    for (auto& pixel : z_buffer) {
        pixel.distance = FloatMax;
        pixel.polygon_id = PixelInfo::no_polygon;
    }
}

}  // namespace renderer
