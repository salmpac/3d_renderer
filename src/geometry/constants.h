#pragma once

#include <limits>

namespace renderer {

using FloatType = float;

inline constexpr FloatType kFloatMax = std::numeric_limits<FloatType>::max();
inline constexpr FloatType kEpsilon = 1e-6f;

}  // namespace renderer
