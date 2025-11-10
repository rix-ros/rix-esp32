#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"

namespace rix {
namespace sensor_msgs {

class ChannelInt32 : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    std::string name{};
    std::vector<int32_t> data{};

    ChannelInt32() = default;
    ChannelInt32(const ChannelInt32 &other) = default;
    ~ChannelInt32() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0x2c848e4a2a124cceULL, 0x98f60930c3252184ULL};
    }

    bool operator==(const ChannelInt32 &other) const {
        if (name != other.name) { return false; }
        if (data != other.data) { return false; }
        return true;
    }

    bool operator!=(const ChannelInt32 &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 2;
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->name, segments, offset);
        detail::get_segments(this->data, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->name, segments, offset);
        detail::get_segments(this->data, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->name);
        len += detail::get_prefix_len(this->data);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->name, dst, offset);
        detail::get_prefix(this->data, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->name, src, offset);
        detail::resize(this->data, src, offset);
        return true;
    }
};

} // namespace sensor_msgs
} // namespace rix