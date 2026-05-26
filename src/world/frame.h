#pragma once

#include <geometry/matrix4x4.h>
#include <geometry/point3d.h>

namespace renderer {

class Frame {
   public:
    Frame() = default;
    Frame(Point3D up_left, Point3D up_right, Point3D down_left, Point3D eye);

    Point3D up_left() const {
        return up_left_;
    }
    Point3D up_right() const {
        return up_right_;
    }
    Point3D down_left() const {
        return down_left_;
    }
    Point3D eye() const {
        return eye_;
    }

    const Matrix4x4& projection_matrix() const {
        return projection_matrix_;
    }

   private:
    void build_projection_matrix();

    Point3D up_left_;
    Point3D up_right_;
    Point3D down_left_;
    Point3D eye_;
    Matrix4x4 projection_matrix_;
};

}  // namespace renderer
