#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"

namespace rix {
namespace sensor_msgs {

class JointState : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    std::string name{};
    double position{};
    double velocity{};
    double effort{};

    JointState() = default;
    JointState(const JointState &other) = default;
    ~JointState() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0x44675cdea46cc6faULL, 0xca0b217bf9b6b11eULL};
    }

    bool operator==(const JointState &other) const {
        if (name != other.name) { return false; }
        if (position != other.position) { return false; }
        if (velocity != other.velocity) { return false; }
        if (effort != other.effort) { return false; }
        return true;
    }

    bool operator!=(const JointState &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 4;
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->name, segments, offset);
        detail::get_segments(this->position, segments, offset);
        detail::get_segments(this->velocity, segments, offset);
        detail::get_segments(this->effort, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->name, segments, offset);
        detail::get_segments(this->position, segments, offset);
        detail::get_segments(this->velocity, segments, offset);
        detail::get_segments(this->effort, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->name);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->name, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->name, src, offset);
        return true;
    }
};

} // namespace sensor_msgs
} // namespace rix