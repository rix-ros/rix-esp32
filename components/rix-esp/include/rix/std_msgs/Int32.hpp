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

class Int32 : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    int32_t data{};

    Int32() = default;
    Int32(const Int32 &other) = default;
    ~Int32() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0xf3cba635445a7a5eULL, 0xc9d460d3dd4a7ba4ULL};
    }

    bool operator==(const Int32 &other) const {
        if (data != other.data) { return false; }
        return true;
    }

    bool operator!=(const Int32 &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 1;
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->data, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->data, segments, offset);
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