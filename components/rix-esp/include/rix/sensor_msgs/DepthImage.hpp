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

class DepthImage : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    std_msgs::Header header{};
    uint32_t width{};
    uint32_t height{};
    std::vector<float> data{};

    DepthImage() = default;
    DepthImage(const DepthImage &other) = default;
    ~DepthImage() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0x21783b2012ac6ec2ULL, 0xa96a2a3516b15bc8ULL};
    }

    bool operator==(const DepthImage &other) const {
        if (header != other.header) { return false; }
        if (width != other.width) { return false; }
        if (height != other.height) { return false; }
        if (data != other.data) { return false; }
        return true;
    }

    bool operator!=(const DepthImage &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 3;
        count += detail::get_segment_count(this->header);
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->header, segments, offset);
        detail::get_segments(this->width, segments, offset);
        detail::get_segments(this->height, segments, offset);
        detail::get_segments(this->data, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->header, segments, offset);
        detail::get_segments(this->width, segments, offset);
        detail::get_segments(this->height, segments, offset);
        detail::get_segments(this->data, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->header);
        len += detail::get_prefix_len(this->data);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->header, dst, offset);
        detail::get_prefix(this->data, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->header, src, offset);
        detail::resize(this->data, src, offset);
        return true;
    }
};

} // namespace sensor_msgs
} // namespace rix