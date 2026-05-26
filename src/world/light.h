#pragma once

#include <geometry/bounding_box.h>
#include <geometry/point3d.h>
#include <world/frame.h>
#include <world/screen.h>
#include <world/z_buffer.h>

#include <vector>

namespace renderer {

class Triangle;

class Light {
   public:
    static Light fit_to(BoundingBox target, Point3D position_hint, Width shadow_width, Height shadow_height);

    Point3D position() const {
        return frame_.eye();
    }
    const Frame& frame() const {
        return frame_;
    }

    void build_shadow_map(const std::vector<Triangle>& triangles);
    bool is_lit(Point3D world_point) const;

   private:
    Light(Frame frame, Width shadow_width, Height shadow_height);

    Frame frame_;
    ZBuffer shadow_map_;
};

}  // namespace renderer
