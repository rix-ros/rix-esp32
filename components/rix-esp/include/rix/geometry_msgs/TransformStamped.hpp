#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"
#include "rix/geometry_msgs/Transform.hpp"
#include "rix/std_msgs/Header.hpp"

namespace rix {
namespace geometry_msgs {

class TransformStamped : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    std_msgs::Header header{};
    std::string child_frame_id{};
    geometry_msgs::Transform transform{};

    TransformStamped() = default;
    TransformStamped(const TransformStamped &other) = default;
    ~TransformStamped() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0x135bac559a7bbd16ULL, 0x071d4d31120d8ec7ULL};
    }

    bool operator==(const TransformStamped &other) const {
        if (header != other.header) { return false; }
        if (child_frame_id != other.child_frame_id) { return false; }
        if (transform != other.transform) { return false; }
        return true;
    }

    bool operator!=(const TransformStamped &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 1;
        count += detail::get_segment_count(this->header);
        count += detail::get_segment_count(this->transform);
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->header, segments, offset);
        detail::get_segments(this->child_frame_id, segments, offset);
        detail::get_segments(this->transform, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->header, segments, offset);
        detail::get_segments(this->child_frame_id, segments, offset);
        detail::get_segments(this->transform, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->header);
        len += detail::get_prefix_len(this->child_frame_id);
        len += detail::get_prefix_len(this->transform);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->header, dst, offset);
        detail::get_prefix(this->child_frame_id, dst, offset);
        detail::get_prefix(this->transform, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->header, src, offset);
        detail::resize(this->child_frame_id, src, offset);
        detail::resize(this->transform, src, offset);
        return true;
    }
};

} // namespace geometry_msgs
} // namespace rix