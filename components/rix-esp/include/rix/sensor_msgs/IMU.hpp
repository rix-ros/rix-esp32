#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"
#include "rix/geometry_msgs/Quaternion.hpp"
#include "rix/geometry_msgs/Vector3.hpp"
#include "rix/std_msgs/Header.hpp"

namespace rix {
namespace sensor_msgs {

class IMU : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    std_msgs::Header header{};
    geometry_msgs::Quaternion orientation{};
    geometry_msgs::Vector3 angular_velocity{};
    geometry_msgs::Vector3 linear_acceleration{};

    IMU() = default;
    IMU(const IMU &other) = default;
    ~IMU() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0xbb2a87e4a4787657ULL, 0x53bacaaac5e27dacULL};
    }

    bool operator==(const IMU &other) const {
        if (header != other.header) { return false; }
        if (orientation != other.orientation) { return false; }
        if (angular_velocity != other.angular_velocity) { return false; }
        if (linear_acceleration != other.linear_acceleration) { return false; }
        return true;
    }

    bool operator!=(const IMU &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 0;
        count += detail::get_segment_count(this->header);
        count += detail::get_segment_count(this->orientation);
        count += detail::get_segment_count(this->angular_velocity);
        count += detail::get_segment_count(this->linear_acceleration);
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->header, segments, offset);
        detail::get_segments(this->orientation, segments, offset);
        detail::get_segments(this->angular_velocity, segments, offset);
        detail::get_segments(this->linear_acceleration, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->header, segments, offset);
        detail::get_segments(this->orientation, segments, offset);
        detail::get_segments(this->angular_velocity, segments, offset);
        detail::get_segments(this->linear_acceleration, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->header);
        len += detail::get_prefix_len(this->orientation);
        len += detail::get_prefix_len(this->angular_velocity);
        len += detail::get_prefix_len(this->linear_acceleration);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->header, dst, offset);
        detail::get_prefix(this->orientation, dst, offset);
        detail::get_prefix(this->angular_velocity, dst, offset);
        detail::get_prefix(this->linear_acceleration, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->header, src, offset);
        detail::resize(this->orientation, src, offset);
        detail::resize(this->angular_velocity, src, offset);
        detail::resize(this->linear_acceleration, src, offset);
        return true;
    }
};

} // namespace sensor_msgs
} // namespace rix