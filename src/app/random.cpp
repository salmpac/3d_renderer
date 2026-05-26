#include <app/random.h>

namespace renderer {

Random::Random(std::uint64_t seed) : generator_(seed) {
}

std::uint8_t Random::uniform_byte(std::uint8_t lo, std::uint8_t hi) {
    std::uniform_int_distribution<int> dist(lo, hi);
    return static_cast<std::uint8_t>(dist(generator_));
}

Color Random::uniform_color(std::uint8_t lo, std::uint8_t hi) {
    return Color{uniform_byte(lo, hi), uniform_byte(lo, hi), uniform_byte(lo, hi)};
}

}  // namespace renderer
