#include <geometry/triangle.h>
#include <world/light.h>
#include <world/rasterizer.h>

#include <algorithm>
#include <cmath>

namespace renderer {

Light::Light(Frame frame, Width shadow_width, Height shadow_height) : frame_(frame), shadow_map_(shadow_width, shadow_height) {
}

Light Light::fit_to(BoundingBox target, Point3D position_hint, Width shadow_width, Height shadow_height) {
    Point3D center = target.empty() ? Point3D{} : target.center();
    FloatType radius = target.empty() ? FloatType{1} : target.max_extent() * FloatType{0.5};
    if (radius < kEpsilon) {
        radius = FloatType{1};
    }

    Point3D direction = Point3D::normalize(center - position_hint);
    if (Point3D::length(direction) < kEpsilon) {
        direction = Point3D{0, 0, -1};
    }

    Point3D world_up{0, 1, 0};
    Point3D right_axis = Point3D::normalize(Point3D::cross(direction, world_up));
    if (Point3D::length(right_axis) < kEpsilon) {
        right_axis = Point3D{1, 0, 0};
    }
    Point3D up_axis = Point3D::normalize(Point3D::cross(right_axis, direction));

    FloatType half_extent = radius * FloatType{2};

    Point3D plane_center = position_hint + direction * Point3D::length(center - position_hint) * FloatType{0.5};
    Point3D up_left = plane_center + up_axis * half_extent - right_axis * half_extent;
    Point3D up_right = plane_center + up_axis * half_extent + right_axis * half_extent;
    Point3D down_left = plane_center - up_axis * half_extent - right_axis * half_extent;

    return Light{Frame{up_left, up_right, down_left, position_hint}, shadow_width, shadow_height};
}

void Light::build_shadow_map(const std::vector<Triangle>& triangles) {
    shadow_map_.clear();
    const Matrix4x4& proj = frame_.projection_matrix();
    FloatType width = static_cast<FloatType>(shadow_map_.width());
    FloatType height = static_cast<FloatType>(shadow_map_.height());

    Rasterizer rasterizer;
    for (const auto& triangle : triangles) {
        Point3D a = proj.apply(triangle.a());
        Point3D b = proj.apply(triangle.b());
        Point3D c = proj.apply(triangle.c());

        Point3D a_screen{(a.x() + 1) * FloatType{0.5} * width, (a.y() + 1) * FloatType{0.5} * height, a.z()};
        Point3D b_screen{(b.x() + 1) * FloatType{0.5} * width, (b.y() + 1) * FloatType{0.5} * height, b.z()};
        Point3D c_screen{(c.x() + 1) * FloatType{0.5} * width, (c.y() + 1) * FloatType{0.5} * height, c.z()};

        rasterizer.rasterize(a_screen, b_screen, c_screen, &shadow_map_, [](int, int, Barycentric) {});
    }
}

bool Light::is_lit(Point3D world_point) const {
    Point4D clip = frame_.projection_matrix().apply(Point4D::from_point(world_point));
    if (clip.w() <= 0) {
        return false;
    }
    Point3D ndc = clip.to_point3d();

    int w = shadow_map_.width();
    int h = shadow_map_.height();

    int center_x = static_cast<int>((ndc.x() + 1) * FloatType{0.5} * w);
    int center_y = static_cast<int>((ndc.y() + 1) * FloatType{0.5} * h);

    if (center_x < 1 || center_x + 1 >= w || center_y < 1 || center_y + 1 >= h) {
        return false;
    }

    FloatType current_depth = ndc.z();
    FloatType bias = FloatType{0.001};

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            FloatType stored = shadow_map_.get(center_x + dx, center_y + dy);
            if (stored == kFloatMax) {
                return true;
            }
            if (current_depth <= stored + bias) {
                return true;
            }
        }
    }
    return false;
}

}  // namespace renderer
