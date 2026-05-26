#include <app/view.h>

#include <stdexcept>

namespace renderer {

View::View(Runtime& runtime, int width, int height) : runtime_(runtime), rgba_(static_cast<std::size_t>(width) * height * 4), width_(width), height_(height) {
    if (!texture_.create(static_cast<unsigned int>(width), static_cast<unsigned int>(height))) {
        throw std::runtime_error("View: failed to create SFML texture");
    }
    sprite_.setTexture(texture_, true);
}

void View::show(const Screen& screen) {
    const Color* pixels = screen.data();
    std::size_t count = static_cast<std::size_t>(width_) * height_;
    for (std::size_t i = 0; i < count; ++i) {
        rgba_[i * 4 + 0] = pixels[i].r();
        rgba_[i * 4 + 1] = pixels[i].g();
        rgba_[i * 4 + 2] = pixels[i].b();
        rgba_[i * 4 + 3] = 255;
    }
    texture_.update(rgba_.data());
    auto& window = runtime_.window();
    window.clear();
    window.draw(sprite_);
    window.display();
}

}  // namespace renderer
