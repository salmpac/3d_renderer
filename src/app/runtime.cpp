#include <app/runtime.h>

namespace renderer {

Runtime::Runtime(unsigned int width, unsigned int height, const char* title) : window_(sf::VideoMode(width, height), title) {
    window_.setFramerateLimit(60);
}

bool Runtime::poll_events() {
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_.close();
            return false;
        }
    }
    return window_.isOpen();
}

}  // namespace renderer
