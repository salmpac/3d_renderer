#include <app/application.h>
#include <geometry/matrix4x4.h>
#include <geometry/point3d.h>
#include <io/obj_reader.h>
#include <world/light.h>
#include <world/mesh.h>

namespace renderer {

Application::Application(std::filesystem::path obj_path)
    : runtime_(kWindowWidth, kWindowHeight, "3D Renderer"), view_(runtime_, kWindowWidth, kWindowHeight),
      camera_(BoundingBox{}, kFovY, static_cast<FloatType>(kWindowWidth) / kWindowHeight), screen_(Width{kWindowWidth}, Height{kWindowHeight}) {
    ObjReader reader{std::move(obj_path)};
    Mesh mesh = reader.read();
    mesh.fit_to_unit_cube();

    BoundingBox bounds = mesh.bounds();
    world_.set_mesh(std::move(mesh));

    FloatType extent = bounds.max_extent();
    Point3D light_position = bounds.center() + Point3D{FloatType{0}, extent * FloatType{3}, FloatType{0}};
    world_.add_light(Light::fit_to(bounds, light_position, Width{kShadowMapSize}, Height{kShadowMapSize}));

    constexpr FloatType aspect = static_cast<FloatType>(kWindowWidth) / kWindowHeight;
    camera_ = Camera{bounds, kFovY, aspect};
}

void Application::run() {
    while (runtime_.poll_events()) {
        angle_ += kRotationStep;
        Matrix4x4 transform = Matrix4x4::rotation_y(angle_);
        renderer_.render(world_, camera_, transform, screen_);
        view_.show(screen_);
    }
}

}  // namespace renderer
