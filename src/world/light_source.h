#pragma once

#include <geometry/primitives.h>
#include <world/oriented_window.h>
#include <world/z_buffer.h>

namespace renderer {

class LightSource : public OrientedWindow {
   public:
    LightSource(const Point4D& up_left, const Point4D& up_right, const Point4D& down_left, const PointOfView& point_of_view, int height, int width)
        : OrientedWindow(up_left, up_right, down_left, point_of_view), z_buffer(height, width) {}

    ~LightSource() = default;
    LightSource(const LightSource&) = default;
    LightSource(LightSource&&) = default;
    LightSource& operator=(const LightSource&) = default;
    LightSource& operator=(LightSource&&) = default;

    bool check_lighted(Point4D point, size_t polygon_index) const;
    void build_z_buffer(const std::vector<Polygon>& polygons);

   private:
    ZBuffer z_buffer;
};

}  // namespace renderer
