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

class Duration : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    int32_t sec{};
    int32_t nsec{};

    Duration() = default;
    Duration(const Duration &other) = default;
    ~Duration() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0x4e0e3d76d0e9f50cULL, 0x18f0feaec29a35deULL};
    }

    bool operator==(const Duration &other) const {
        if (sec != other.sec) { return false; }
        if (nsec != other.nsec) { return false; }
        return true;
    }

    bool operator!=(const Duration &other) const {
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