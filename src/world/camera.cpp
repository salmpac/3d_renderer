#include <world/camera.h>

#include <cmath>

namespace renderer {

Camera::Camera(BoundingBox target, FloatType fov_y, FloatType aspect) {
    Point3D center = target.empty() ? Point3D{} : target.center();
    FloatType radius = target.empty() ? FloatType{1} : target.max_extent() * FloatType{0.5};
    if (radius < kEpsilon) {
        radius = FloatType{1};
    }

    FloatType half_h = radius;
    FloatType half_w = radius * aspect;

    FloatType distance = half_h / std::tan(fov_y * FloatType{0.5});
    FloatType margin = FloatType{1.5};
    distance *= margin;

    Point3D eye{center.x(), center.y(), center.z() + distance};

    FloatType window_distance = distance / margin;
    FloatType near_plane_z = center.z() + distance - window_distance;

    Point3D up_left{center.x() - half_w, center.y() + half_h, near_plane_z};
    Point3D up_right{center.x() + half_w, center.y() + half_h, near_plane_z};
    Point3D down_left{center.x() - half_w, center.y() - half_h, near_plane_z};

    frame_ = Frame{up_left, up_right, down_left, eye};
}

}  // namespace renderer
