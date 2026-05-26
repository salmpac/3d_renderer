#pragma once

#include <geometry/constants.h>
#include <geometry/point3d.h>
#include <geometry/point4d.h>

#include <initializer_list>

namespace renderer {

class Matrix4x4 {
   public:
    Matrix4x4();
    Matrix4x4(std::initializer_list<std::initializer_list<FloatType>> rows);

    static Matrix4x4 identity();
    static Matrix4x4 translation(Point3D offset);
    static Matrix4x4 rotation_y(FloatType angle);
    static Matrix4x4 scaling(FloatType factor);

    FloatType& operator()(int row, int col);
    FloatType operator()(int row, int col) const;

    Matrix4x4& operator*=(const Matrix4x4& rhs);

    Point4D apply(Point4D point) const;
    Point3D apply(Point3D point) const;  // converts via Point4D, homogenizes

   private:
    FloatType data_[4][4];
};

Matrix4x4 operator*(Matrix4x4 lhs, const Matrix4x4& rhs);

}  // namespace renderer
