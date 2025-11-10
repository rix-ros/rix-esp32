#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"
#include "rix/geometry_msgs/Point.hpp"
#include "rix/geometry_msgs/Quaternion.hpp"

namespace rix {
namespace geometry_msgs {

class Pose : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    geometry_msgs::Point position{};
    geometry_msgs::Quaternion orientation{};

    Pose() = default;
    Pose(const Pose &other) = default;
    ~Pose() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0x3475bb2a37017521ULL, 0x92d334780b8bf5edULL};
    }

    bool operator==(const Pose &other) const {
        if (position != other.position) { return false; }
        if (orientation != other.orientation) { return false; }
        return true;
    }

    bool operator!=(const Pose &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 0;
        count += detail::get_segment_count(this->position);
        count += detail::get_segment_count(this->orientation);
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->position, segments, offset);
        detail::get_segments(this->orientation, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->position, segments, offset);
        detail::get_segments(this->orientation, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->position);
        len += detail::get_prefix_len(this->orientation);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->position, dst, offset);
        detail::get_prefix(this->orientation, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->position, src, offset);
        detail::resize(this->orientation, src, offset);
        return true;
    }
};

} // namespace geometry_msgs
} // namespace rix