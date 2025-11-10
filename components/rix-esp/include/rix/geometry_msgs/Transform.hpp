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

namespace rix {
namespace geometry_msgs {

class Transform : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    geometry_msgs::Vector3 translation{};
    geometry_msgs::Quaternion rotation{};

    Transform() = default;
    Transform(const Transform &other) = default;
    ~Transform() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0xe72bb4114fb11acaULL, 0xa418e989b4269592ULL};
    }

    bool operator==(const Transform &other) const {
        if (translation != other.translation) { return false; }
        if (rotation != other.rotation) { return false; }
        return true;
    }

    bool operator!=(const Transform &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 0;
        count += detail::get_segment_count(this->translation);
        count += detail::get_segment_count(this->rotation);
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->translation, segments, offset);
        detail::get_segments(this->rotation, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->translation, segments, offset);
        detail::get_segments(this->rotation, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->translation);
        len += detail::get_prefix_len(this->rotation);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->translation, dst, offset);
        detail::get_prefix(this->rotation, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->translation, src, offset);
        detail::resize(this->rotation, src, offset);
        return true;
    }
};

} // namespace geometry_msgs
} // namespace rix