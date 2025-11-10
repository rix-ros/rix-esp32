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

class Status : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    uint64_t id{};
    uint8_t error{};

    Status() = default;
    Status(const Status &other) = default;
    ~Status() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0xa623ac8714eac8f3ULL, 0x796852390665f97aULL};
    }

    bool operator==(const Status &other) const {
        if (id != other.id) { return false; }
        if (error != other.error) { return false; }
        return true;
    }

    bool operator!=(const Status &other) const {
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
        detail::get_segments(this->id, segments, offset);
        detail::get_segments(this->error, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->id, segments, offset);
        detail::get_segments(this->error, segments, offset);
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