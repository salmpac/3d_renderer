#include <app/application.h>
#include <iostream>

int main(int argc, char** argv) {
    try {
        std::filesystem::path obj_path = (argc > 1) ? argv[1] : "cow.obj";
        renderer::Application app{std::move(obj_path)};
        app.run();
    }catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Error: unknown exception" << std::endl;
        return 1;
    }
    return 0;
}
