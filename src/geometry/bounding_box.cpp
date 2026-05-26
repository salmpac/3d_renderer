#include <geometry/bounding_box.h>

#include <algorithm>

namespace renderer {

BoundingBox::BoundingBox() : min_{kFloatMax, kFloatMax, kFloatMax}, max_{-kFloatMax, -kFloatMax, -kFloatMax} {
}

BoundingBox::BoundingBox(Point3D min, Point3D max) : min_(min), max_(max), empty_(false) {
}

void BoundingBox::expand(Point3D point) {
    if (empty_) {
        min_ = point;
        max_ = point;
        empty_ = false;
        return;
    }
    min_.x() = std::min(min_.x(), point.x());
    min_.y() = std::min(min_.y(), point.y());
    min_.z() = std::min(min_.z(), point.z());
    max_.x() = std::max(max_.x(), point.x());
    max_.y() = std::max(max_.y(), point.y());
    max_.z() = std::max(max_.z(), point.z());
}

Point3D BoundingBox::center() const {
    return (min_ + max_) * FloatType{0.5};
}

Point3D BoundingBox::extent() const {
    return max_ - min_;
}

FloatType BoundingBox::max_extent() const {
    Point3D e = extent();
    return std::max({e.x(), e.y(), e.z()});
}

}  // namespace renderer
