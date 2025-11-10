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

class ParamInfo : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    uint64_t id{};
    std::string name{};
    std::array<uint64_t, 2> message_hash{};
    std::vector<uint8_t> data{};

    ParamInfo() = default;
    ParamInfo(const ParamInfo &other) = default;
    ~ParamInfo() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0x2de46e1102566ce6ULL, 0x6845b8baa5d9a103ULL};
    }

    bool operator==(const ParamInfo &other) const {
        if (id != other.id) { return false; }
        if (name != other.name) { return false; }
        if (message_hash != other.message_hash) { return false; }
        if (data != other.data) { return false; }
        return true;
    }

    bool operator!=(const ParamInfo &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 4;
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->id, segments, offset);
        detail::get_segments(this->name, segments, offset);
        detail::get_segments(this->message_hash, segments, offset);
        detail::get_segments(this->data, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->id, segments, offset);
        detail::get_segments(this->name, segments, offset);
        detail::get_segments(this->message_hash, segments, offset);
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

} // namespace sys_msgs
} // namespace rix