#pragma once

#include <geometry/bounding_box.h>
#include <geometry/constants.h>
#include <geometry/matrix4x4.h>
#include <geometry/point3d.h>
#include <world/frame.h>

namespace renderer {

class Camera {
   public:
    Camera(BoundingBox target, FloatType fov_y, FloatType aspect);

    Point3D eye() const {
        return frame_.eye();
    }
    const Frame& frame() const {
        return frame_;
    }
    const Matrix4x4& projection_matrix() const {
        return frame_.projection_matrix();
    }

   private:
    Frame frame_;
};

}  // namespace renderer
