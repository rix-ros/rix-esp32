#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"
#include "rix/geometry_msgs/Pose.hpp"
#include "rix/std_msgs/Header.hpp"

namespace rix {
namespace geometry_msgs {

class PoseStamped : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    std_msgs::Header header{};
    geometry_msgs::Pose pose{};

    PoseStamped() = default;
    PoseStamped(const PoseStamped &other) = default;
    ~PoseStamped() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0x8f31a213bf4183ceULL, 0xc9142c29899ae4b5ULL};
    }

    bool operator==(const PoseStamped &other) const {
        if (header != other.header) { return false; }
        if (pose != other.pose) { return false; }
        return true;
    }

    bool operator!=(const PoseStamped &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 0;
        count += detail::get_segment_count(this->header);
        count += detail::get_segment_count(this->pose);
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->header, segments, offset);
        detail::get_segments(this->pose, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->header, segments, offset);
        detail::get_segments(this->pose, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->header);
        len += detail::get_prefix_len(this->pose);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->header, dst, offset);
        detail::get_prefix(this->pose, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->header, src, offset);
        detail::resize(this->pose, src, offset);
        return true;
    }
};

} // namespace geometry_msgs
} // namespace rix