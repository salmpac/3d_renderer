#pragma once

#include <app/runtime.h>
#include <world/screen.h>

#include <SFML/Graphics.hpp>
#include <vector>

namespace renderer {

class View {
   public:
    View(Runtime& runtime, int width, int height);

    void show(const Screen& screen);

   private:
    Runtime& runtime_;
    sf::Texture texture_;
    sf::Sprite sprite_;
    std::vector<std::uint8_t> rgba_;
    int width_;
    int height_;
};

}  // namespace renderer
