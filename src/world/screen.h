#pragma once

#include <geometry/primitives.h>
#include <world/color_info.h>
#include <world/light_source.h>
#include <world/oriented_window.h>
#include <world/z_buffer.h>

#include <vector>

namespace renderer {

class Screen : public OrientedWindow {
   public:
    Screen(const Point4D& up_left, const Point4D& up_right, const Point4D& down_left, const PointOfView& point_of_view, int height, int width)
        : OrientedWindow(up_left, up_right, down_left, point_of_view), z_buffer(height, width) {}

    ~Screen() = default;
    Screen(const Screen&) = default;
    Screen(Screen&&) = default;
    Screen& operator=(const Screen&) = default;
    Screen& operator=(Screen&&) = default;

    template <typename PictureType>
        requires HasSetColorMethod<PictureType>
    void draw_picture(PictureType& picture, const std::vector<LightSource>& light_sources, const std::vector<Polygon>& polygons) {
        z_buffer.clear();

        FloatType w = z_buffer.get_width();
        FloatType h = z_buffer.get_height();

        for (size_t i = 0; i < polygons.size(); ++i) {
            Polygon p = polygons[i];

            p.A = (projection_matrix * p.A).homogenize();
            p.B = (projection_matrix * p.B).homogenize();
            p.C = (projection_matrix * p.C).homogenize();

            p.A[0] = (p.A[0] + 1.0f) * 0.5f * w;
            p.A[1] = (p.A[1] + 1.0f) * 0.5f * h;
            p.B[0] = (p.B[0] + 1.0f) * 0.5f * w;
            p.B[1] = (p.B[1] + 1.0f) * 0.5f * h;
            p.C[0] = (p.C[0] + 1.0f) * 0.5f * w;
            p.C[1] = (p.C[1] + 1.0f) * 0.5f * h;

            z_buffer.add_polygon(p, i);
        }

        for (int y = 0; y < (int)h; ++y) {
            for (int x = 0; x < (int)w; ++x) {
                auto pixel_info = z_buffer.get_pixel_info(x, y);

                if (pixel_info.polygon_id == ZBuffer::PixelInfo::no_polygon) {
                    picture.set(x, y, {0, 0, 0});
                    continue;
                }

                const Polygon& poly = polygons[pixel_info.polygon_id];

                Point4D sA = (projection_matrix * poly.A).homogenize();
                Point4D sB = (projection_matrix * poly.B).homogenize();
                Point4D sC = (projection_matrix * poly.C).homogenize();
                Point4D pA((sA[0] + 1.0f) * 0.5f * w, (sA[1] + 1.0f) * 0.5f * h, sA[2]);
                Point4D pB((sB[0] + 1.0f) * 0.5f * w, (sB[1] + 1.0f) * 0.5f * h, sB[2]);
                Point4D pC((sC[0] + 1.0f) * 0.5f * w, (sC[1] + 1.0f) * 0.5f * h, sC[2]);

                Point4D pix((FloatType)x + 0.5f, (FloatType)y + 0.5f, 0);
                FloatType area = (pC[0] - pA[0]) * (pB[1] - pA[1]) - (pC[1] - pA[1]) * (pB[0] - pA[0]);
                FloatType w0 = ((pB[1] - pC[1]) * (pix[0] - pC[0]) + (pC[0] - pB[0]) * (pix[1] - pC[1])) / area;
                FloatType w1 = ((pC[1] - pA[1]) * (pix[0] - pC[0]) + (pA[0] - pC[0]) * (pix[1] - pC[1])) / area;
                FloatType w2 = 1.0f - w0 - w1;

                Point4D world_pos = poly.A * w0 + poly.B * w1 + poly.C * w2;

                float intensity = 0.5f; 

                for (const auto& light : light_sources) {
                    if (light.check_lighted(world_pos, pixel_info.polygon_id)) {
                        intensity += 0.5f;
                    }
                }
                if (intensity > 1.0f) {
                    intensity = 1.0f;
                }

                Color final_color = {(uint8_t)(poly.color.r * intensity), (uint8_t)(poly.color.g * intensity), (uint8_t)(poly.color.b * intensity)};
                picture.set(x, y, {final_color.r, final_color.g, final_color.b});
            }
        }
    }

   private:
    ZBuffer z_buffer;
};

}  // namespace renderer
