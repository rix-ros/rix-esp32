#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"

namespace rix {
namespace std_msgs {

class Color : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    float r{};
    float g{};
    float b{};
    float a{};

    Color() = default;
    Color(const Color &other) = default;
    ~Color() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0x3a7c60442e56c6d5ULL, 0x5860858b9073af8dULL};
    }

    bool operator==(const Color &other) const {
        if (r != other.r) { return false; }
        if (g != other.g) { return false; }
        if (b != other.b) { return false; }
        if (a != other.a) { return false; }
        return true;
    }

    bool operator!=(const Color &other) const {
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
        detail::get_segments(this->r, segments, offset);
        detail::get_segments(this->g, segments, offset);
        detail::get_segments(this->b, segments, offset);
        detail::get_segments(this->a, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->r, segments, offset);
        detail::get_segments(this->g, segments, offset);
        detail::get_segments(this->b, segments, offset);
        detail::get_segments(this->a, segments, offset);
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

} // namespace std_msgs
} // namespace rix