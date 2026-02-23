#pragma once

#include <geometry/primitives.h>
#include <world/view.h>

namespace renderer {

class OrientedWindow {
   public:
    OrientedWindow(const Point4D& up_left, const Point4D& up_right, const Point4D& down_left, const PointOfView& point_of_view)
        : up_left(up_left), up_right(up_right), down_left(down_left), point_of_view(point_of_view) {
        build_projection_matrix();
    }

    ~OrientedWindow() = default;
    OrientedWindow(const OrientedWindow&) = default;
    OrientedWindow(OrientedWindow&&) = default;
    OrientedWindow& operator=(const OrientedWindow&) = default;
    OrientedWindow& operator=(OrientedWindow&&) = default;

   protected:
    void build_projection_matrix();

    Point4D up_left;
    Point4D up_right;
    Point4D down_left;
    PointOfView point_of_view;
    Matrix4x4 projection_matrix;
};

}  // namespace renderer
