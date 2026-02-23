#include <world/light_source.h>

namespace renderer {

void LightSource::build_z_buffer(const std::vector<Polygon>& polygons) {
    z_buffer.clear();

    for (size_t i = 0; i < polygons.size(); ++i) {
        Polygon transformed_poly = polygons[i];

        transformed_poly.A = projection_matrix * polygons[i].A;
        transformed_poly.B = projection_matrix * polygons[i].B;
        transformed_poly.C = projection_matrix * polygons[i].C;

        transformed_poly.A = transformed_poly.A.homogenize();
        transformed_poly.B = transformed_poly.B.homogenize();
        transformed_poly.C = transformed_poly.C.homogenize();

        FloatType w = z_buffer.get_width();
        FloatType h = z_buffer.get_height();
        transformed_poly.A[0] = (transformed_poly.A[0] + 1.0f) * 0.5f * w;
        transformed_poly.A[1] = (transformed_poly.A[1] + 1.0f) * 0.5f * h;
        transformed_poly.B[0] = (transformed_poly.B[0] + 1.0f) * 0.5f * w;
        transformed_poly.B[1] = (transformed_poly.B[1] + 1.0f) * 0.5f * h;
        transformed_poly.C[0] = (transformed_poly.C[0] + 1.0f) * 0.5f * w;
        transformed_poly.C[1] = (transformed_poly.C[1] + 1.0f) * 0.5f * h;

        z_buffer.add_polygon(transformed_poly, i);
    }
}

bool LightSource::check_lighted(Point4D point, [[maybe_unused]] size_t polygon_index) const {
    Point4D light_clip = projection_matrix * point;
    Point4D light_ndc = light_clip.homogenize();
    if (light_clip[3] <= 0) {
        return false;
    }

    int w = z_buffer.get_width();
    int h = z_buffer.get_height();

    int center_x = (int)((light_ndc[0] + 1.0f) * 0.5f * w);
    int center_y = (int)((light_ndc[1] + 1.0f) * 0.5f * h);

    if (center_x < 0 || center_x + 1 >= w || center_y < 0 || center_y + 1 >= h) {
        return false;
    }

    FloatType current_depth = light_ndc[2];

    bool is_visible = false;

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            auto pixel_info = z_buffer.get_pixel_info(center_x + dx, center_y + dy);

            if (pixel_info.distance == FloatMax) {
                is_visible = true;
            } else if (current_depth >= pixel_info.distance - 0.00003f && current_depth <= pixel_info.distance + 0.00003f) {
                is_visible = true;
            }
        }
    }

    return is_visible;
}

}  // namespace renderer
