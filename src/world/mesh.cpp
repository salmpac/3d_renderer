#include <world/mesh.h>

namespace renderer {

void Mesh::add(Triangle triangle) {
    bounds_.expand(triangle.a());
    bounds_.expand(triangle.b());
    bounds_.expand(triangle.c());
    triangles_.push_back(triangle);
}

void Mesh::fit_to_unit_cube() {
    if (bounds_.empty()) {
        return;
    }
    Point3D center = bounds_.center();
    FloatType scale_factor = bounds_.max_extent();
    if (scale_factor < kEpsilon) {
        return;
    }
    FloatType inv_scale = FloatType{1} / scale_factor;

    Matrix4x4 normalize = Matrix4x4::scaling(inv_scale) * Matrix4x4::translation(-center);

    BoundingBox new_bounds;
    for (auto& triangle : triangles_) {
        triangle = triangle.transformed(normalize);
        new_bounds.expand(triangle.a());
        new_bounds.expand(triangle.b());
        new_bounds.expand(triangle.c());
    }
    bounds_ = new_bounds;
}

std::vector<Triangle> Mesh::transformed(const Matrix4x4& matrix) const {
    std::vector<Triangle> result;
    result.reserve(triangles_.size());
    for (const auto& triangle : triangles_) {
        result.push_back(triangle.transformed(matrix));
    }
    return result;
}

}  // namespace renderer
