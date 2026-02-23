#pragma once

#include <world/color_info.h>

#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <limits>

namespace renderer {

using FloatType = float;
constexpr FloatType FloatMax = std::numeric_limits<FloatType>::max();
constexpr FloatType EPSILON = 1e-6f;

struct Point4D {
    FloatType coordinates[4];

    inline Point4D() : coordinates{0, 0, 0, 0} {}

    inline Point4D(FloatType x, FloatType y, FloatType z, FloatType w = 1.0f) : coordinates{x, y, z, w} {}

    inline FloatType& operator[](int index) { return coordinates[index]; }

    inline const FloatType& operator[](int index) const { return coordinates[index]; }

    inline Point4D homogenize() const {
        if (std::abs(coordinates[3] - 1.0f) < EPSILON || std::abs(coordinates[3]) < EPSILON) {
            return *this;
        }
        FloatType invW = 1.0f / coordinates[3];
        return {coordinates[0] * invW, coordinates[1] * invW, coordinates[2] * invW, 1.0f};
    }

    static inline FloatType dot(const Point4D& a, const Point4D& b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }

    static inline FloatType length(const Point4D& v) { return std::sqrt(dot(v, v)); }

    static inline Point4D normalize(const Point4D& v) {
        FloatType len = length(v);
        if (len < EPSILON)
            return {0, 0, 0, 0};
        FloatType invLen = 1.0f / len;
        return {v[0] * invLen, v[1] * invLen, v[2] * invLen, 0.0f};
    }

    static inline Point4D cross(const Point4D& a, const Point4D& b) {
        return {a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0], 0.0f};
    }
};

inline Point4D operator+(const Point4D& lhs, const Point4D& rhs) {
    return {lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2], lhs[3] + rhs[3]};
}

inline Point4D operator-(const Point4D& lhs, const Point4D& rhs) {
    return {lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2], lhs[3] - rhs[3]};
}

inline Point4D operator*(const Point4D& v, FloatType s) {
    return {v[0] * s, v[1] * s, v[2] * s, v[3] * s};
}

inline Point4D operator*(FloatType s, const Point4D& v) {
    return v * s;
}

inline Point4D operator/(const Point4D& v, FloatType s) {
    FloatType invS = 1.0f / s;
    return {v[0] * invS, v[1] * invS, v[2] * invS, v[3] * invS};
}

struct Polygon {
    Point4D A;
    Point4D B;
    Point4D C;
    Color color;
    // Point4D normalA, normalB, normalC; // TODO
};

struct Matrix4x4 {
    FloatType matrix[4][4];

    inline Matrix4x4() {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                matrix[i][j] = 0;
            }
        }
    }

    static inline Matrix4x4 Identity() {
        Matrix4x4 res;
        res.matrix[0][0] = 1.0f;
        res.matrix[1][1] = 1.0f;
        res.matrix[2][2] = 1.0f;
        res.matrix[3][3] = 1.0f;
        return res;
    }

    inline Matrix4x4(std::initializer_list<std::initializer_list<FloatType>> list) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                matrix[i][j] = 0;
            }
        }

        int i = 0;
        for (const auto& row : list) {
            if (i >= 4) {
                break;
            }
            int j = 0;
            for (const auto& value : row) {
                if (j >= 4) {
                    break;
                }
                matrix[i][j] = value;
                ++j;
            }
            ++i;
        }
    }

    inline FloatType* operator[](int index) { return matrix[index]; }

    inline const FloatType* operator[](int index) const { return matrix[index]; }

    inline Matrix4x4 operator*(const Matrix4x4& other) const {
        Matrix4x4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                FloatType sum = 0;
                for (int k = 0; k < 4; ++k) {
                    sum += matrix[i][k] * other[k][j];
                }
                result[i][j] = sum;
            }
        }
        return result;
    }

    inline Point4D operator*(const Point4D& point) const {
        Point4D result(0, 0, 0, 0);
        for (int i = 0; i < 4; ++i) {
            FloatType sum = 0;
            for (int j = 0; j < 4; ++j) {
                sum += matrix[i][j] * point[j];
            }
            result[i] = sum;
        }
        return result;
    }
};

}  // namespace renderer
