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

class Time : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    uint32_t sec{};
    uint32_t nsec{};

    Time() = default;
    Time(const Time &other) = default;
    ~Time() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0x2c986df177cf5907ULL, 0x77d96a49131d2c31ULL};
    }

    bool operator==(const Time &other) const {
        if (sec != other.sec) { return false; }
        if (nsec != other.nsec) { return false; }
        return true;
    }

    bool operator!=(const Time &other) const {
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
        detail::get_segments(this->sec, segments, offset);
        detail::get_segments(this->nsec, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->sec, segments, offset);
        detail::get_segments(this->nsec, segments, offset);
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