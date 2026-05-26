#include <geometry/point3d.h>

#include <cmath>

namespace renderer {

Point3D::Point3D(FloatType x, FloatType y, FloatType z) : x_(x), y_(y), z_(z) {
}

Point3D& Point3D::operator+=(Point3D rhs) {
    x_ += rhs.x_;
    y_ += rhs.y_;
    z_ += rhs.z_;
    return *this;
}

Point3D& Point3D::operator-=(Point3D rhs) {
    x_ -= rhs.x_;
    y_ -= rhs.y_;
    z_ -= rhs.z_;
    return *this;
}

Point3D& Point3D::operator*=(FloatType scalar) {
    x_ *= scalar;
    y_ *= scalar;
    z_ *= scalar;
    return *this;
}

Point3D& Point3D::operator/=(FloatType scalar) {
    FloatType inv = FloatType{1} / scalar;
    return *this *= inv;
}

FloatType Point3D::dot(Point3D a, Point3D b) {
    return a.x_ * b.x_ + a.y_ * b.y_ + a.z_ * b.z_;
}

Point3D Point3D::cross(Point3D a, Point3D b) {
    return Point3D{a.y_ * b.z_ - a.z_ * b.y_, a.z_ * b.x_ - a.x_ * b.z_, a.x_ * b.y_ - a.y_ * b.x_};
}

FloatType Point3D::length(Point3D v) {
    return std::sqrt(dot(v, v));
}

Point3D Point3D::normalize(Point3D v) {
    FloatType len = length(v);
    if (len < kEpsilon) {
        return Point3D{};
    }
    return v / len;
}

Point3D operator+(Point3D lhs, Point3D rhs) {
    return lhs += rhs;
}

Point3D operator-(Point3D lhs, Point3D rhs) {
    return lhs -= rhs;
}

Point3D operator-(Point3D v) {
    return Point3D{-v.x(), -v.y(), -v.z()};
}

Point3D operator*(Point3D v, FloatType scalar) {
    return v *= scalar;
}

Point3D operator*(FloatType scalar, Point3D v) {
    return v *= scalar;
}

Point3D operator/(Point3D v, FloatType scalar) {
    return v /= scalar;
}

}  // namespace renderer
