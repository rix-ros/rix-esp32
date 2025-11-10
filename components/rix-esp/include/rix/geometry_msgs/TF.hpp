#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"
#include "rix/geometry_msgs/TransformStamped.hpp"

namespace rix {
namespace geometry_msgs {

class TF : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    std::vector<geometry_msgs::TransformStamped> transforms{};

    TF() = default;
    TF(const TF &other) = default;
    ~TF() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0x8a7abf1eb6fdee97ULL, 0x09b57186e481669eULL};
    }

    bool operator==(const TF &other) const {
        if (transforms != other.transforms) { return false; }
        return true;
    }

    bool operator!=(const TF &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 0;
        count += detail::get_segment_count(this->transforms);
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->transforms, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->transforms, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->transforms);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->transforms, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->transforms, src, offset);
        return true;
    }
};

} // namespace geometry_msgs
} // namespace rix