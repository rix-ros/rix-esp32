#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"
#include "rix/geometry_msgs/Vector3.hpp"

namespace rix {
namespace geometry_msgs {

class Twist : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    geometry_msgs::Vector3 linear{};
    geometry_msgs::Vector3 angular{};

    Twist() = default;
    Twist(const Twist &other) = default;
    ~Twist() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0x93bc972bf1c0a644ULL, 0xc5f5a2c99771f58aULL};
    }

    bool operator==(const Twist &other) const {
        if (linear != other.linear) { return false; }
        if (angular != other.angular) { return false; }
        return true;
    }

    bool operator!=(const Twist &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 0;
        count += detail::get_segment_count(this->linear);
        count += detail::get_segment_count(this->angular);
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->linear, segments, offset);
        detail::get_segments(this->angular, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->linear, segments, offset);
        detail::get_segments(this->angular, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->linear);
        len += detail::get_prefix_len(this->angular);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->linear, dst, offset);
        detail::get_prefix(this->angular, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->linear, src, offset);
        detail::resize(this->angular, src, offset);
        return true;
    }
};

} // namespace geometry_msgs
} // namespace rix