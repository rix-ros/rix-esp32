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

class WheelEncoder : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    std_msgs::Header header{};
    std::vector<int64_t> ticks{};
    std::vector<int32_t> delta_ticks{};
    int32_t delta_time{};

    WheelEncoder() = default;
    WheelEncoder(const WheelEncoder &other) = default;
    ~WheelEncoder() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0x398cccfc867fc8b9ULL, 0x0d145c993003b6dbULL};
    }

    bool operator==(const WheelEncoder &other) const {
        if (header != other.header) { return false; }
        if (ticks != other.ticks) { return false; }
        if (delta_ticks != other.delta_ticks) { return false; }
        if (delta_time != other.delta_time) { return false; }
        return true;
    }

    bool operator!=(const WheelEncoder &other) const {
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
        detail::get_segments(this->ticks, segments, offset);
        detail::get_segments(this->delta_ticks, segments, offset);
        detail::get_segments(this->delta_time, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->header, segments, offset);
        detail::get_segments(this->ticks, segments, offset);
        detail::get_segments(this->delta_ticks, segments, offset);
        detail::get_segments(this->delta_time, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->header);
        len += detail::get_prefix_len(this->ticks);
        len += detail::get_prefix_len(this->delta_ticks);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->header, dst, offset);
        detail::get_prefix(this->ticks, dst, offset);
        detail::get_prefix(this->delta_ticks, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->header, src, offset);
        detail::resize(this->ticks, src, offset);
        detail::resize(this->delta_ticks, src, offset);
        return true;
    }
};

} // namespace sensor_msgs
} // namespace rix