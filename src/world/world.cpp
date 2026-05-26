#include <world/world.h>

namespace renderer {

void World::set_mesh(Mesh mesh) {
    mesh_ = std::move(mesh);
}

void World::add_light(Light light) {
    lights_.push_back(std::move(light));
}

}  // namespace renderer
