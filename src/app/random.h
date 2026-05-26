#pragma once

#include <world/color.h>

#include <cstdint>
#include <random>

namespace renderer {

class Random {
   public:
    explicit Random(std::uint64_t seed = 42);

    std::uint8_t uniform_byte(std::uint8_t lo, std::uint8_t hi);
    Color uniform_color(std::uint8_t lo = 55, std::uint8_t hi = 255);

   private:
    std::mt19937_64 generator_;
};

}  // namespace renderer
