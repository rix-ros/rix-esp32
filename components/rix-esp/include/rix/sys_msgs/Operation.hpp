#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"

namespace rix {
namespace sys_msgs {

class Operation : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    uint32_t len{};
    uint8_t opcode{};

    Operation() = default;
    Operation(const Operation &other) = default;
    ~Operation() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0xf2659fdc53838877ULL, 0xaa12b902c2bad1a7ULL};
    }

    bool operator==(const Operation &other) const {
        if (len != other.len) { return false; }
        if (opcode != other.opcode) { return false; }
        return true;
    }

    bool operator!=(const Operation &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 2;
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->len, segments, offset);
        detail::get_segments(this->opcode, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->len, segments, offset);
        detail::get_segments(this->opcode, segments, offset);
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

} // namespace sys_msgs
} // namespace rix