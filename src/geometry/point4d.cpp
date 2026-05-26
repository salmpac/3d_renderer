#include <geometry/point4d.h>

#include <cmath>

namespace renderer {

Point4D::Point4D(FloatType x, FloatType y, FloatType z, FloatType w) : x_(x), y_(y), z_(z), w_(w) {
}

Point4D Point4D::from_point(Point3D p) {
    return Point4D{p.x(), p.y(), p.z(), FloatType{1}};
}

Point4D Point4D::from_direction(Point3D v) {
    return Point4D{v.x(), v.y(), v.z(), FloatType{0}};
}

Point3D Point4D::to_point3d() const {
    if (std::abs(w_) < kEpsilon) {
        return Point3D{x_, y_, z_};
    }
    FloatType inv_w = FloatType{1} / w_;
    return Point3D{x_ * inv_w, y_ * inv_w, z_ * inv_w};
}

}  // namespace renderer
