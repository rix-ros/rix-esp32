#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"
#include "rix/std_msgs/Header.hpp"

namespace rix {
namespace sensor_msgs {

class MotorPWM : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    std_msgs::Header header{};
    std::vector<float> pwm{};

    MotorPWM() = default;
    MotorPWM(const MotorPWM &other) = default;
    ~MotorPWM() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0x08706ff8a3e3c65aULL, 0x42b7c25b643a1b83ULL};
    }

    bool operator==(const MotorPWM &other) const {
        if (header != other.header) { return false; }
        if (pwm != other.pwm) { return false; }
        return true;
    }

    bool operator!=(const MotorPWM &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 1;
        count += detail::get_segment_count(this->header);
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->header, segments, offset);
        detail::get_segments(this->pwm, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->header, segments, offset);
        detail::get_segments(this->pwm, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->header);
        len += detail::get_prefix_len(this->pwm);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->header, dst, offset);
        detail::get_prefix(this->pwm, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->header, src, offset);
        detail::resize(this->pwm, src, offset);
        return true;
    }
};

} // namespace sensor_msgs
} // namespace rix