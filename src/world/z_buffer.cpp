#include <world/z_buffer.h>

#include <algorithm>

namespace renderer {

ZBuffer::ZBuffer(Width width, Height height)
    : width_(static_cast<int>(width)), height_(static_cast<int>(height)), depth_(static_cast<std::size_t>(width_) * height_, kFloatMax) {
}

void ZBuffer::clear() {
    std::fill(depth_.begin(), depth_.end(), kFloatMax);
}

FloatType ZBuffer::get(int x, int y) const {
    return depth_[static_cast<std::size_t>(y) * width_ + x];
}

bool ZBuffer::test_and_set(int x, int y, FloatType depth) {
    std::size_t idx = static_cast<std::size_t>(y) * width_ + x;
    if (depth < depth_[idx]) {
        depth_[idx] = depth;
        return true;
    }
    return false;
}

}  // namespace renderer
