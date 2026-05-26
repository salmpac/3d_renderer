#pragma once

#include <app/runtime.h>
#include <app/view.h>
#include <geometry/constants.h>
#include <world/camera.h>
#include <world/renderer.h>
#include <world/screen.h>
#include <world/world.h>

#include <filesystem>

namespace renderer {

class Application {
   public:
    explicit Application(std::filesystem::path obj_path);

    void run();

   private:
    static constexpr int kWindowWidth = 1280;
    static constexpr int kWindowHeight = 720;
    static constexpr int kShadowMapSize = 1024;
    static constexpr FloatType kFovY = FloatType{0.785398};  // 45 in radians
    static constexpr FloatType kRotationStep = FloatType{0.03};

    Runtime runtime_;
    View view_;
    World world_;
    Camera camera_;
    Renderer renderer_;
    Screen screen_;
    FloatType angle_ = 0;
};

}  // namespace renderer
