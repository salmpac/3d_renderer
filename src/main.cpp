#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Подключаем твои хедеры
#include <geometry/primitives.h>
#include <world/light_source.h>
#include <world/screen.h>

using namespace renderer;

// ==========================================
// 1. Адаптер для буфера SFML
// ==========================================
struct SFMLBuffer {
    std::vector<uint8_t>& pixels;
    int width;
    int height;

    void set(int x, int y, Color color) {
        if (x < 0 || x >= width || y < 0 || y >= height) return;

        // SFML использует RGBA
        size_t index = (y * width + x) * 4;
        pixels[index + 0] = color.r;
        pixels[index + 1] = color.g;
        pixels[index + 2] = color.b;
        pixels[index + 3] = 255;
    }
};

// ==========================================
// 2. Загрузка OBJ
// ==========================================
std::vector<Polygon> load_obj(const std::string& filename) {
    std::vector<Polygon> polygons;
    std::vector<Point4D> vertices;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return {};
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            float x, y, z;
            ss >> x >> y >> z;
            vertices.emplace_back(x, y, z);
        } else if (prefix == "f") {
            int idx[3];
            for (int i = 0; i < 3; ++i) {
                std::string vertex_str;
                ss >> vertex_str;
                size_t slash = vertex_str.find('/');
                if (slash != std::string::npos) {
                    idx[i] = std::stoi(vertex_str.substr(0, slash)) - 1;
                } else {
                    idx[i] = std::stoi(vertex_str) - 1;
                }
            }
            Polygon p;
            p.A = vertices[idx[0]];
            p.B = vertices[idx[1]];
            p.C = vertices[idx[2]];
            // Случайный цвет
            p.color = {(uint8_t)(rand() % 200 + 55), (uint8_t)(rand() % 200 + 55), (uint8_t)(rand() % 200 + 55)};
            polygons.push_back(p);
        }
    }
    std::cout << "Model loaded: " << polygons.size() << " polygons." << std::endl;
    return polygons;
}

std::vector<Polygon> create_cube() {
    std::vector<Polygon> polys;
    std::vector<Point4D> v = {
        {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
        {-1, -1, 1},  {1, -1, 1},  {1, 1, 1},  {-1, 1, 1}
    };
    int indices[][3] = {
        {0, 1, 2}, {2, 3, 0}, {1, 5, 6}, {6, 2, 1}, {7, 6, 5}, {5, 4, 7},
        {4, 0, 3}, {3, 7, 4}, {4, 5, 1}, {1, 0, 4}, {3, 2, 6}, {6, 7, 3}
    };
    for (auto& idx : indices) {
        Polygon p; p.A = v[idx[0]]; p.B = v[idx[1]]; p.C = v[idx[2]];
        p.color = {(uint8_t)(rand() % 255), (uint8_t)(rand() % 255), (uint8_t)(rand() % 255)};
        polys.push_back(p);
    }
    return polys;
}

// ==========================================
// 3. MAIN
// ==========================================
int main() {
    const int WIDTH = 1920;
    const int HEIGHT = 1080;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "3D Engine Renderer");
    window.setFramerateLimit(60);

    sf::Texture texture;
    texture.create(WIDTH, HEIGHT);
    sf::Sprite sprite(texture);
    std::vector<uint8_t> pixels(WIDTH * HEIGHT * 4);

    // ================= НАСТРОЙКИ МАСШТАБА =================
    // 1. Увеличить саму модель (если она в файле очень мелкая)
    float model_scale = 0.05f; 

    // 2. "Бинокль" камеры. 
    // 1.0 = как было (широкий угол). 
    // 2.0 = в 2 раза крупнее. 
    // 4.0 = в 4 раза крупнее.
    float camera_zoom = 3.0f; 
    // ======================================================


    // -- 1. Загрузка Сцены --
    std::vector<Polygon> base_model = load_obj("/Users/salmpac/projects/3d_render/cow.obj");
    if (base_model.empty()) base_model = create_cube();

    // Применяем model_scale
    if (std::abs(model_scale - 1.0f) > 0.001f) {
        for (auto& poly : base_model) {
            poly.A = poly.A * model_scale; poly.A[3] = 1.0f;
            poly.B = poly.B * model_scale; poly.B[3] = 1.0f;
            poly.C = poly.C * model_scale; poly.C[3] = 1.0f;
        }
    }

    // -- 2. Настройка Камеры --
    Point4D eye(0, 0, 3);
    float screen_z_pos = 2.5f; // Расстояние от глаза = 0.5 (3.0 - 2.5)

    // Базовые размеры окна (которые у тебя были)
    float base_half_w = 0.4f; // 0.2 было 
    float base_half_h = 0.3f; // 0.15 было

    // Применяем зум к окну проекции
    float hw = (base_half_w / 2.0f) / camera_zoom; // делим на 2 потому что ты просил сузить базово
    float hh = (base_half_h / 2.0f) / camera_zoom;

    Point4D up_left(-hw, hh, screen_z_pos);
    Point4D up_right(hw, hh, screen_z_pos);
    Point4D down_left(-hw, -hh, screen_z_pos);

    PointOfView cam_pov = {eye};
    Screen screen(up_left, up_right, down_left, cam_pov, HEIGHT, WIDTH);


    // -- 3. Настройка Света --
    // Свет ставим подальше, чтобы он был как солнце
    Point4D light_eye(5, 5, 5);
    PointOfView light_pov = {light_eye};
    
    // Окно света должно быть большим, чтобы охватить всю сцену в центре
    Point4D l_ul(-10, 10, 2); 
    Point4D l_ur( 10, 10, 2);
    Point4D l_dl(-10, -10, 2);

    LightSource light(l_ul, l_ur, l_dl, light_pov, WIDTH, HEIGHT);
    std::vector<LightSource> lights;

    // Вращение
    float angle = 0.0f;

    // == ГЛАВНЫЙ ЦИКЛ ==
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        angle += 0.03f;

        // Копия для трансформации
        std::vector<Polygon> polygons_to_draw = base_model;

        // Матрица вращения Y
        Matrix4x4 rotY;
        rotY[0][0] = cos(angle);  rotY[0][2] = sin(angle);
        rotY[1][1] = 1;
        rotY[2][0] = -sin(angle); rotY[2][2] = cos(angle);
        rotY[3][3] = 1;

        for (auto& poly : polygons_to_draw) {
            poly.A = rotY * poly.A;
            poly.B = rotY * poly.B;
            poly.C = rotY * poly.C;
        }

        // Очистка
        std::fill(pixels.begin(), pixels.end(), 0);

        // Shadow Pass
        light.build_z_buffer(polygons_to_draw);
        lights.clear();
        lights.push_back(light);

        // Render Pass
        SFMLBuffer adapter = {pixels, WIDTH, HEIGHT};
        screen.draw_picture(adapter, lights, polygons_to_draw);

        // Display
        texture.update(pixels.data());
        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
