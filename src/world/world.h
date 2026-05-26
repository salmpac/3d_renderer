#pragma once

#include <geometry/bounding_box.h>
#include <world/light.h>
#include <world/mesh.h>

#include <vector>

namespace renderer {

class World {
   public:
    World() = default;

    void set_mesh(Mesh mesh);
    void add_light(Light light);

    const Mesh& mesh() const {
        return mesh_;
    }
    const std::vector<Light>& lights() const {
        return lights_;
    }
    std::vector<Light>& lights() {
        return lights_;
    }

    BoundingBox bounds() const {
        return mesh_.bounds();
    }

   private:
    Mesh mesh_;
    std::vector<Light> lights_;
};

}  // namespace renderer
