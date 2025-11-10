#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"
#include "rix/geometry_msgs/Vector3.hpp"

namespace rix {
namespace geometry_msgs {

class Inertia : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    double mass{};
    geometry_msgs::Vector3 center_of_mass{};
    double ixx{};
    double ixy{};
    double ixz{};
    double iyy{};
    double iyz{};
    double izz{};

    Inertia() = default;
    Inertia(const Inertia &other) = default;
    ~Inertia() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0xd6b8ac1caaf29fedULL, 0x3aa199ef6ae6b22dULL};
    }

    bool operator==(const Inertia &other) const {
        if (mass != other.mass) { return false; }
        if (center_of_mass != other.center_of_mass) { return false; }
        if (ixx != other.ixx) { return false; }
        if (ixy != other.ixy) { return false; }
        if (ixz != other.ixz) { return false; }
        if (iyy != other.iyy) { return false; }
        if (iyz != other.iyz) { return false; }
        if (izz != other.izz) { return false; }
        return true;
    }

    bool operator!=(const Inertia &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 7;
        count += detail::get_segment_count(this->center_of_mass);
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->mass, segments, offset);
        detail::get_segments(this->center_of_mass, segments, offset);
        detail::get_segments(this->ixx, segments, offset);
        detail::get_segments(this->ixy, segments, offset);
        detail::get_segments(this->ixz, segments, offset);
        detail::get_segments(this->iyy, segments, offset);
        detail::get_segments(this->iyz, segments, offset);
        detail::get_segments(this->izz, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->mass, segments, offset);
        detail::get_segments(this->center_of_mass, segments, offset);
        detail::get_segments(this->ixx, segments, offset);
        detail::get_segments(this->ixy, segments, offset);
        detail::get_segments(this->ixz, segments, offset);
        detail::get_segments(this->iyy, segments, offset);
        detail::get_segments(this->iyz, segments, offset);
        detail::get_segments(this->izz, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->center_of_mass);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->center_of_mass, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->center_of_mass, src, offset);
        return true;
    }
};

} // namespace geometry_msgs
} // namespace rix