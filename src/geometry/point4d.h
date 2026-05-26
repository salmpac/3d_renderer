#pragma once

#include <geometry/constants.h>
#include <geometry/point3d.h>

namespace renderer {

// Homogeneous coordinates in RP^3.
class Point4D {
   public:
    Point4D() = default;
    Point4D(FloatType x, FloatType y, FloatType z, FloatType w);

    static Point4D from_point(Point3D p);      // w = 1
    static Point4D from_direction(Point3D v);  // w = 0

    FloatType x() const {
        return x_;
    }
    FloatType y() const {
        return y_;
    }
    FloatType z() const {
        return z_;
    }
    FloatType w() const {
        return w_;
    }

    Point3D to_point3d() const;  // homogenize, then drop w

   private:
    FloatType x_ = 0;
    FloatType y_ = 0;
    FloatType z_ = 0;
    FloatType w_ = 0;
};

}  // namespace renderer
