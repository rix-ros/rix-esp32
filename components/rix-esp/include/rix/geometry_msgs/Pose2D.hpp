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

class Pose2D : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    float x{};
    float y{};
    float theta{};

    Pose2D() = default;
    Pose2D(const Pose2D &other) = default;
    ~Pose2D() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0x0d6b986e4a106d9bULL, 0xe6dfe44a89086508ULL};
    }

    bool operator==(const Pose2D &other) const {
        if (x != other.x) { return false; }
        if (y != other.y) { return false; }
        if (theta != other.theta) { return false; }
        return true;
    }

    bool operator!=(const Pose2D &other) const {
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
        detail::get_segments(this->x, segments, offset);
        detail::get_segments(this->y, segments, offset);
        detail::get_segments(this->theta, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->x, segments, offset);
        detail::get_segments(this->y, segments, offset);
        detail::get_segments(this->theta, segments, offset);
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