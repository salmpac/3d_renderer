#pragma once

#include <SFML/Graphics.hpp>

namespace renderer {

class Runtime {
   public:
    Runtime(unsigned int width, unsigned int height, const char* title);

    sf::RenderWindow& window() {
        return window_;
    }
    bool is_open() const {
        return window_.isOpen();
    }

    bool poll_events();

   private:
    sf::RenderWindow window_;
};

}  // namespace renderer
