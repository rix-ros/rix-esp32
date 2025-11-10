#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"

namespace rix {
namespace geometry_msgs {

class Twist2D : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    float vx{};
    float vy{};
    float wz{};

    Twist2D() = default;
    Twist2D(const Twist2D &other) = default;
    ~Twist2D() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0xafdea30da49b1e62ULL, 0x6490e357e767e107ULL};
    }

    bool operator==(const Twist2D &other) const {
        if (vx != other.vx) { return false; }
        if (vy != other.vy) { return false; }
        if (wz != other.wz) { return false; }
        return true;
    }

    bool operator!=(const Twist2D &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 3;
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->vx, segments, offset);
        detail::get_segments(this->vy, segments, offset);
        detail::get_segments(this->wz, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->vx, segments, offset);
        detail::get_segments(this->vy, segments, offset);
        detail::get_segments(this->wz, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        
        return true;
    }
};

} // namespace geometry_msgs
} // namespace rix