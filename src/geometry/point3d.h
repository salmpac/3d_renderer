#pragma once

#include <geometry/constants.h>

namespace renderer {

class Point3D {
   public:
    Point3D() = default;
    Point3D(FloatType x, FloatType y, FloatType z);

    FloatType x() const {
        return x_;
    }
    FloatType y() const {
        return y_;
    }
    FloatType z() const {
        return z_;
    }

    FloatType& x() {
        return x_;
    }
    FloatType& y() {
        return y_;
    }
    FloatType& z() {
        return z_;
    }

    Point3D& operator+=(Point3D rhs);
    Point3D& operator-=(Point3D rhs);
    Point3D& operator*=(FloatType scalar);
    Point3D& operator/=(FloatType scalar);

    static FloatType dot(Point3D a, Point3D b);
    static Point3D cross(Point3D a, Point3D b);
    static FloatType length(Point3D v);
    static Point3D normalize(Point3D v);

   private:
    FloatType x_ = 0;
    FloatType y_ = 0;
    FloatType z_ = 0;
};

Point3D operator+(Point3D lhs, Point3D rhs);
Point3D operator-(Point3D lhs, Point3D rhs);
Point3D operator-(Point3D v);
Point3D operator*(Point3D v, FloatType scalar);
Point3D operator*(FloatType scalar, Point3D v);
Point3D operator/(Point3D v, FloatType scalar);

}  // namespace renderer
