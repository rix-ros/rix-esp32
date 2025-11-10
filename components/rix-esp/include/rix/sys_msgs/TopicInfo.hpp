#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"

namespace rix {
namespace sys_msgs {

class TopicInfo : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    std::string name{};
    std::array<uint64_t, 2> message_hash{};

    TopicInfo() = default;
    TopicInfo(const TopicInfo &other) = default;
    ~TopicInfo() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0xbfec3f6ae39a4ebaULL, 0xa2c9c2475241c698ULL};
    }

    bool operator==(const TopicInfo &other) const {
        if (name != other.name) { return false; }
        if (message_hash != other.message_hash) { return false; }
        return true;
    }

    bool operator!=(const TopicInfo &other) const {
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
        detail::get_segments(this->message_hash, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->name, segments, offset);
        detail::get_segments(this->message_hash, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->name);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->name, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->name, src, offset);
        return true;
    }
};

} // namespace sys_msgs
} // namespace rix