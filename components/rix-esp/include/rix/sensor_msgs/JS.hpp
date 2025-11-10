#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"
#include "rix/sensor_msgs/JointState.hpp"
#include "rix/std_msgs/Time.hpp"

namespace rix {
namespace sensor_msgs {

class JS : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    std_msgs::Time stamp{};
    std::vector<sensor_msgs::JointState> joint_states{};

    JS() = default;
    JS(const JS &other) = default;
    ~JS() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0x55e949d83a339123ULL, 0x3466cecf532db579ULL};
    }

    bool operator==(const JS &other) const {
        if (stamp != other.stamp) { return false; }
        if (joint_states != other.joint_states) { return false; }
        return true;
    }

    bool operator!=(const JS &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 0;
        count += detail::get_segment_count(this->stamp);
        count += detail::get_segment_count(this->joint_states);
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->stamp, segments, offset);
        detail::get_segments(this->joint_states, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->stamp, segments, offset);
        detail::get_segments(this->joint_states, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->stamp);
        len += detail::get_prefix_len(this->joint_states);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->stamp, dst, offset);
        detail::get_prefix(this->joint_states, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->stamp, src, offset);
        detail::resize(this->joint_states, src, offset);
        return true;
    }
};

} // namespace sensor_msgs
} // namespace rix