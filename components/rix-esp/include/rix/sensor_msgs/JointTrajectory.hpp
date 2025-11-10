#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"
#include "rix/sensor_msgs/JS.hpp"

namespace rix {
namespace sensor_msgs {

class JointTrajectory : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    std::vector<sensor_msgs::JS> points{};

    JointTrajectory() = default;
    JointTrajectory(const JointTrajectory &other) = default;
    ~JointTrajectory() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0xb8d8c6a9c366e39cULL, 0x910ecf6c1c3357b4ULL};
    }

    bool operator==(const JointTrajectory &other) const {
        if (points != other.points) { return false; }
        return true;
    }

    bool operator!=(const JointTrajectory &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 0;
        count += detail::get_segment_count(this->points);
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->points, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->points, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->points);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->points, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->points, src, offset);
        return true;
    }
};

} // namespace sensor_msgs
} // namespace rix