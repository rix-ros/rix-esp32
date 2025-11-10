#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"
#include "rix/geometry_msgs/Vector3.hpp"
#include "rix/std_msgs/Header.hpp"

namespace rix {
namespace geometry_msgs {

class Vector3Stamped : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    std_msgs::Header header{};
    geometry_msgs::Vector3 vector3{};

    Vector3Stamped() = default;
    Vector3Stamped(const Vector3Stamped &other) = default;
    ~Vector3Stamped() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0x5aa40e2d4b671d4dULL, 0x6f77f894baafd65eULL};
    }

    bool operator==(const Vector3Stamped &other) const {
        if (header != other.header) { return false; }
        if (vector3 != other.vector3) { return false; }
        return true;
    }

    bool operator!=(const Vector3Stamped &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 0;
        count += detail::get_segment_count(this->header);
        count += detail::get_segment_count(this->vector3);
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->header, segments, offset);
        detail::get_segments(this->vector3, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->header, segments, offset);
        detail::get_segments(this->vector3, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->header);
        len += detail::get_prefix_len(this->vector3);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->header, dst, offset);
        detail::get_prefix(this->vector3, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->header, src, offset);
        detail::resize(this->vector3, src, offset);
        return true;
    }
};

} // namespace geometry_msgs
} // namespace rix