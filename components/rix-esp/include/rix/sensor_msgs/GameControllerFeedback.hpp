#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"
#include "rix/std_msgs/Duration.hpp"
#include "rix/std_msgs/Header.hpp"

namespace rix {
namespace sensor_msgs {

class GameControllerFeedback : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    std_msgs::Header header{};
    std::vector<float> intensities{};
    std::vector<std_msgs::Duration> durations{};

    GameControllerFeedback() = default;
    GameControllerFeedback(const GameControllerFeedback &other) = default;
    ~GameControllerFeedback() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0xd54cb857e19ff0dbULL, 0xa4c0f1ecd372a64bULL};
    }

    bool operator==(const GameControllerFeedback &other) const {
        if (header != other.header) { return false; }
        if (intensities != other.intensities) { return false; }
        if (durations != other.durations) { return false; }
        return true;
    }

    bool operator!=(const GameControllerFeedback &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 1;
        count += detail::get_segment_count(this->header);
        count += detail::get_segment_count(this->durations);
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->header, segments, offset);
        detail::get_segments(this->intensities, segments, offset);
        detail::get_segments(this->durations, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->header, segments, offset);
        detail::get_segments(this->intensities, segments, offset);
        detail::get_segments(this->durations, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->header);
        len += detail::get_prefix_len(this->intensities);
        len += detail::get_prefix_len(this->durations);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->header, dst, offset);
        detail::get_prefix(this->intensities, dst, offset);
        detail::get_prefix(this->durations, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->header, src, offset);
        detail::resize(this->intensities, src, offset);
        detail::resize(this->durations, src, offset);
        return true;
    }
};

} // namespace sensor_msgs
} // namespace rix