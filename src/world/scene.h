#pragma once

#include <geometry/primitives.h>

#include <vector>

namespace renderer {

// scene in absolute coordinates
class Scene {
   public:
    Scene() = default;
    ~Scene() = default;
    Scene(const Scene&) = default;
    Scene(Scene&&) = default;
    Scene& operator=(const Scene&) = default;
    Scene& operator=(Scene&&) = default;

   private:
    // TODO: maybe use vector of objects, each containing vector of polygons
    std::vector<Polygon> polygons;
};

}  // namespace renderer
