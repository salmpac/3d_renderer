#include <world/oriented_window.h>

namespace renderer {

void OrientedWindow::build_projection_matrix() {
    Point4D eye = point_of_view.position;

    Point4D vec_right = up_right - up_left;
    Point4D vec_down = down_left - up_left;

    Point4D X = Point4D::normalize(vec_right);
    Point4D Y = Point4D::normalize(vec_down);
    Point4D Z = Point4D::normalize(Point4D::cross(X, Y));

    Matrix4x4 view = {{X[0], X[1], X[2], -Point4D::dot(X, eye)},
                      {Y[0], Y[1], Y[2], -Point4D::dot(Y, eye)},
                      {Z[0], Z[1], Z[2], -Point4D::dot(Z, eye)},
                      {0.0f, 0.0f, 0.0f, 1.0f}};


    FloatType dist = Point4D::dot(up_left - eye, Z);
    if (dist < 0.001f) {
        dist = 0.001f;
    }

    FloatType width_phys = Point4D::length(vec_right);
    FloatType height_phys = Point4D::length(vec_down);

    float z_near = 0.1f;
    float z_far = 100.0f;

    Matrix4x4 projection = {{(2.0f * dist) / width_phys, 0, 0, 0},
                            {0, (2.0f * dist) / height_phys, 0, 0},
                            {0, 0, z_far / (z_far - z_near), -(z_far * z_near) / (z_far - z_near)},
                            {0, 0, 1.0f, 0}};

    projection_matrix = projection * view;
}

}  // namespace renderer
