#include <world/renderer.h>

#include <geometry/triangle.h>
#include <world/camera.h>
#include <world/light.h>
#include <world/world.h>
#include <world/z_buffer.h>

#include <algorithm>

namespace renderer {

namespace {

Point3D project_to_screen(const Matrix4x4& projection, Point3D point, FloatType width, FloatType height) {
    Point4D clip = projection.apply(Point4D::from_point(point));
    Point3D ndc = clip.to_point3d();
    return Point3D{(ndc.x() + 1) * FloatType{0.5} * width, (ndc.y() + 1) * FloatType{0.5} * height, ndc.z()};
}

}  // namespace

void Renderer::render(World& world, const Camera& camera, const Matrix4x4& world_transform, Screen& screen) {
    std::vector<Triangle> transformed = world.mesh().transformed(world_transform);

    for (auto& light : world.lights()) {
        light.build_shadow_map(transformed);
    }

    screen.fill(Color{0, 0, 0});
    ZBuffer depth{Width{screen.width()}, Height{screen.height()}};

    FloatType screen_w = static_cast<FloatType>(screen.width());
    FloatType screen_h = static_cast<FloatType>(screen.height());

    const auto& lights = world.lights();
    const Matrix4x4& proj = camera.projection_matrix();
    const FloatType ambient = FloatType{0.3};
    const FloatType diffuse_step = lights.empty() ? FloatType{0} : (FloatType{1} - ambient) / static_cast<FloatType>(lights.size());

    for (const auto& triangle : transformed) {
        Point3D a_screen = project_to_screen(proj, triangle.a(), screen_w, screen_h);
        Point3D b_screen = project_to_screen(proj, triangle.b(), screen_w, screen_h);
        Point3D c_screen = project_to_screen(proj, triangle.c(), screen_w, screen_h);

        Point3D world_a = triangle.a();
        Point3D world_b = triangle.b();
        Point3D world_c = triangle.c();
        Color color = triangle.color();

        rasterizer_.rasterize(a_screen, b_screen, c_screen, &depth, [&](int x, int y, Barycentric bary) {
            Point3D world_pos = world_a * bary.w0 + world_b * bary.w1 + world_c * bary.w2;
            FloatType intensity = ambient;
            for (const auto& light : lights) {
                if (light.is_lit(world_pos)) {
                    intensity += diffuse_step;
                }
            }
            intensity = std::min(intensity, FloatType{1});
            screen.set(x, y, color * intensity);
        });
    }
}

}  // namespace renderer
