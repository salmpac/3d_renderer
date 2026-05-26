#pragma once

#include <geometry/bounding_box.h>
#include <geometry/matrix4x4.h>
#include <geometry/triangle.h>

#include <vector>

namespace renderer {

class Mesh {
   public:
    Mesh() = default;

    void add(Triangle triangle);

    const std::vector<Triangle>& triangles() const {
        return triangles_;
    }
    const BoundingBox& bounds() const {
        return bounds_;
    }

    // Recenter to origin and rescale so the longest extent equals 1.
    void fit_to_unit_cube();

    // Returns triangles with matrix applied (does not mutate this mesh).
    std::vector<Triangle> transformed(const Matrix4x4& matrix) const;

   private:
    std::vector<Triangle> triangles_;
    BoundingBox bounds_;
};

}  // namespace renderer
