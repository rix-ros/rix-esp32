#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"
#include "rix/geometry_msgs/Point.hpp"
#include "rix/sensor_msgs/ChannelFloat.hpp"
#include "rix/std_msgs/Header.hpp"

namespace rix {
namespace sensor_msgs {

class PointCloud : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    std_msgs::Header header{};
    std::vector<geometry_msgs::Point> points{};
    std::vector<sensor_msgs::ChannelFloat> channels{};

    PointCloud() = default;
    PointCloud(const PointCloud &other) = default;
    ~PointCloud() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0xc301ad19cf585118ULL, 0x690f8e2e5094ef18ULL};
    }

    bool operator==(const PointCloud &other) const {
        if (header != other.header) { return false; }
        if (points != other.points) { return false; }
        if (channels != other.channels) { return false; }
        return true;
    }

    bool operator!=(const PointCloud &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 0;
        count += detail::get_segment_count(this->header);
        count += detail::get_segment_count(this->points);
        count += detail::get_segment_count(this->channels);
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->header, segments, offset);
        detail::get_segments(this->points, segments, offset);
        detail::get_segments(this->channels, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->header, segments, offset);
        detail::get_segments(this->points, segments, offset);
        detail::get_segments(this->channels, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->header);
        len += detail::get_prefix_len(this->points);
        len += detail::get_prefix_len(this->channels);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->header, dst, offset);
        detail::get_prefix(this->points, dst, offset);
        detail::get_prefix(this->channels, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->header, src, offset);
        detail::resize(this->points, src, offset);
        detail::resize(this->channels, src, offset);
        return true;
    }
};

} // namespace sensor_msgs
} // namespace rix