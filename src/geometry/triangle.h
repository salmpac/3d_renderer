#pragma once

#include <geometry/bounding_box.h>
#include <geometry/matrix4x4.h>
#include <geometry/point3d.h>
#include <world/color.h>

namespace renderer {

class Triangle {
   public:
    Triangle() = default;
    Triangle(Point3D a, Point3D b, Point3D c, Color color);

    Point3D a() const {
        return a_;
    }
    Point3D b() const {
        return b_;
    }
    Point3D c() const {
        return c_;
    }
    Color color() const {
        return color_;
    }

    BoundingBox bounds() const;
    Triangle transformed(const Matrix4x4& matrix) const;

   private:
    Point3D a_;
    Point3D b_;
    Point3D c_;
    Color color_;
};

}  // namespace renderer
