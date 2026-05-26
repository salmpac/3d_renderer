#pragma once

#include <geometry/matrix4x4.h>
#include <world/rasterizer.h>
#include <world/screen.h>

namespace renderer {

class Camera;
class World;

class Renderer {
   public:
    void render(World& world, const Camera& camera, const Matrix4x4& world_transform, Screen& screen);

   private:
    Rasterizer rasterizer_;
};

}  // namespace renderer
