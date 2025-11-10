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

class Quaternion : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    float w{};
    float x{};
    float y{};
    float z{};

    Quaternion() = default;
    Quaternion(const Quaternion &other) = default;
    ~Quaternion() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0x83793865bdc238a2ULL, 0x09c98f9410cc55f2ULL};
    }

    bool operator==(const Quaternion &other) const {
        if (w != other.w) { return false; }
        if (x != other.x) { return false; }
        if (y != other.y) { return false; }
        if (z != other.z) { return false; }
        return true;
    }

    bool operator!=(const Quaternion &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 4;
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->w, segments, offset);
        detail::get_segments(this->x, segments, offset);
        detail::get_segments(this->y, segments, offset);
        detail::get_segments(this->z, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->w, segments, offset);
        detail::get_segments(this->x, segments, offset);
        detail::get_segments(this->y, segments, offset);
        detail::get_segments(this->z, segments, offset);
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