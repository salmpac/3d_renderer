#include <world/frame.h>

namespace renderer {

Frame::Frame(Point3D up_left, Point3D up_right, Point3D down_left, Point3D eye) : up_left_(up_left), up_right_(up_right), down_left_(down_left), eye_(eye) {
    build_projection_matrix();
}

void Frame::build_projection_matrix() {
    Point3D vec_right = up_right_ - up_left_;
    Point3D vec_down = down_left_ - up_left_;

    Point3D axis_x = Point3D::normalize(vec_right);
    Point3D axis_y = Point3D::normalize(vec_down);
    Point3D axis_z = Point3D::normalize(Point3D::cross(axis_x, axis_y));

    Matrix4x4 view{{axis_x.x(), axis_x.y(), axis_x.z(), -Point3D::dot(axis_x, eye_)},
                   {axis_y.x(), axis_y.y(), axis_y.z(), -Point3D::dot(axis_y, eye_)},
                   {axis_z.x(), axis_z.y(), axis_z.z(), -Point3D::dot(axis_z, eye_)},
                   {0, 0, 0, 1}};

    FloatType dist = Point3D::dot(up_left_ - eye_, axis_z);
    if (dist < FloatType{0.001}) {
        dist = FloatType{0.001};
    }

    FloatType width = Point3D::length(vec_right);
    FloatType height = Point3D::length(vec_down);

    FloatType z_near = FloatType{0.1};
    FloatType z_far = FloatType{1000};

    Matrix4x4 projection{{FloatType{2} * dist / width, 0, 0, 0},
                         {0, FloatType{2} * dist / height, 0, 0},
                         {0, 0, z_far / (z_far - z_near), -(z_far * z_near) / (z_far - z_near)},
                         {0, 0, 1, 0}};

    projection_matrix_ = projection * view;
}

}  // namespace renderer
