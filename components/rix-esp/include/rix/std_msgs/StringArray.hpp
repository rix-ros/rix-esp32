#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"

namespace rix {
namespace std_msgs {

class StringArray : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    std::vector<std::string> data{};

    StringArray() = default;
    StringArray(const StringArray &other) = default;
    ~StringArray() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0x0b0037e24d1ca1bdULL, 0xdcd02f361fcd39e6ULL};
    }

    bool operator==(const StringArray &other) const {
        if (data != other.data) { return false; }
        return true;
    }

    bool operator!=(const StringArray &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 0;
        count += detail::get_segment_count(this->data);
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->data, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->data, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->data);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->data, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->data, src, offset);
        return true;
    }
};

} // namespace std_msgs
} // namespace rix