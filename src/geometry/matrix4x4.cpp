#include <geometry/matrix4x4.h>

#include <cmath>

namespace renderer {

Matrix4x4::Matrix4x4() : data_{{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}} {}

Matrix4x4::Matrix4x4(std::initializer_list<std::initializer_list<FloatType>> rows) : Matrix4x4() {
    int i = 0;
    for (const auto& row : rows) {
        if (i >= 4) {
            break;
        }
        int j = 0;
        for (FloatType value : row) {
            if (j >= 4) {
                break;
            }
            data_[i][j] = value;
            ++j;
        }
        ++i;
    }
}

Matrix4x4 Matrix4x4::identity() {
    Matrix4x4 result;
    for (int i = 0; i < 4; ++i) {
        result.data_[i][i] = FloatType{1};
    }
    return result;
}

Matrix4x4 Matrix4x4::translation(Point3D offset) {
    Matrix4x4 result = identity();
    result(0, 3) = offset.x();
    result(1, 3) = offset.y();
    result(2, 3) = offset.z();
    return result;
}

Matrix4x4 Matrix4x4::rotation_y(FloatType angle) {
    FloatType c = std::cos(angle);
    FloatType s = std::sin(angle);
    return Matrix4x4{{c, 0, s, 0}, {0, 1, 0, 0}, {-s, 0, c, 0}, {0, 0, 0, 1}};
}

Matrix4x4 Matrix4x4::scaling(FloatType factor) {
    return Matrix4x4{{factor, 0, 0, 0}, {0, factor, 0, 0}, {0, 0, factor, 0}, {0, 0, 0, 1}};
}

FloatType& Matrix4x4::operator()(int row, int col) {
    return data_[row][col];
}

FloatType Matrix4x4::operator()(int row, int col) const {
    return data_[row][col];
}

Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& rhs) {
    Matrix4x4 result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            FloatType sum = 0;
            for (int k = 0; k < 4; ++k) {
                sum += data_[i][k] * rhs.data_[k][j];
            }
            result.data_[i][j] = sum;
        }
    }
    *this = result;
    return *this;
}

Point4D Matrix4x4::apply(Point4D point) const {
    FloatType v[4] = {point.x(), point.y(), point.z(), point.w()};
    FloatType result[4] = {0, 0, 0, 0};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result[i] += data_[i][j] * v[j];
        }
    }
    return Point4D{result[0], result[1], result[2], result[3]};
}

Point3D Matrix4x4::apply(Point3D point) const {
    return apply(Point4D::from_point(point)).to_point3d();
}

Matrix4x4 operator*(Matrix4x4 lhs, const Matrix4x4& rhs) {
    return lhs *= rhs;
}

}  // namespace renderer
