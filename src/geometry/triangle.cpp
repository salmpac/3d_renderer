#include <geometry/triangle.h>

namespace renderer {

Triangle::Triangle(Point3D a, Point3D b, Point3D c, Color color) : a_(a), b_(b), c_(c), color_(color) {}

BoundingBox Triangle::bounds() const {
    BoundingBox box;
    box.expand(a_);
    box.expand(b_);
    box.expand(c_);
    return box;
}

Triangle Triangle::transformed(const Matrix4x4& matrix) const {
    return Triangle{matrix.apply(a_), matrix.apply(b_), matrix.apply(c_), color_};
}

}  // namespace renderer
