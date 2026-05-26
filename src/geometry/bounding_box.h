#pragma once

#include <geometry/point3d.h>

namespace renderer {

class BoundingBox {
   public:
    BoundingBox();
    BoundingBox(Point3D min, Point3D max);

    void expand(Point3D point);

    Point3D min() const {
        return min_;
    }
    Point3D max() const {
        return max_;
    }
    Point3D center() const;
    Point3D extent() const;
    FloatType max_extent() const;
    bool empty() const {
        return empty_;
    }

   private:
    Point3D min_;
    Point3D max_;
    bool empty_ = true;
};

}  // namespace renderer
