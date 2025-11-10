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

class LaserScan : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    std_msgs::Header header{};
    float angle_min{};
    float angle_max{};
    float angle_increment{};
    float time_increment{};
    float scan_time{};
    float range_min{};
    float range_max{};
    std::vector<float> ranges{};
    std::vector<float> intensities{};

    LaserScan() = default;
    LaserScan(const LaserScan &other) = default;
    ~LaserScan() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0xab23b6ca54536077ULL, 0x970e000fa7213a6dULL};
    }

    bool operator==(const LaserScan &other) const {
        if (header != other.header) { return false; }
        if (angle_min != other.angle_min) { return false; }
        if (angle_max != other.angle_max) { return false; }
        if (angle_increment != other.angle_increment) { return false; }
        if (time_increment != other.time_increment) { return false; }
        if (scan_time != other.scan_time) { return false; }
        if (range_min != other.range_min) { return false; }
        if (range_max != other.range_max) { return false; }
        if (ranges != other.ranges) { return false; }
        if (intensities != other.intensities) { return false; }
        return true;
    }

    bool operator!=(const LaserScan &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 9;
        count += detail::get_segment_count(this->header);
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->header, segments, offset);
        detail::get_segments(this->angle_min, segments, offset);
        detail::get_segments(this->angle_max, segments, offset);
        detail::get_segments(this->angle_increment, segments, offset);
        detail::get_segments(this->time_increment, segments, offset);
        detail::get_segments(this->scan_time, segments, offset);
        detail::get_segments(this->range_min, segments, offset);
        detail::get_segments(this->range_max, segments, offset);
        detail::get_segments(this->ranges, segments, offset);
        detail::get_segments(this->intensities, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->header, segments, offset);
        detail::get_segments(this->angle_min, segments, offset);
        detail::get_segments(this->angle_max, segments, offset);
        detail::get_segments(this->angle_increment, segments, offset);
        detail::get_segments(this->time_increment, segments, offset);
        detail::get_segments(this->scan_time, segments, offset);
        detail::get_segments(this->range_min, segments, offset);
        detail::get_segments(this->range_max, segments, offset);
        detail::get_segments(this->ranges, segments, offset);
        detail::get_segments(this->intensities, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->header);
        len += detail::get_prefix_len(this->ranges);
        len += detail::get_prefix_len(this->intensities);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->header, dst, offset);
        detail::get_prefix(this->ranges, dst, offset);
        detail::get_prefix(this->intensities, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->header, src, offset);
        detail::resize(this->ranges, src, offset);
        detail::resize(this->intensities, src, offset);
        return true;
    }
};

} // namespace sensor_msgs
} // namespace rix