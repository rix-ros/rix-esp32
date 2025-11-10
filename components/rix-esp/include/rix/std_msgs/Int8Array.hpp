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

class Int8Array : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    std::vector<int8_t> data{};

    Int8Array() = default;
    Int8Array(const Int8Array &other) = default;
    ~Int8Array() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0x48248287fd661fbeULL, 0x6998be51e4cea379ULL};
    }

    bool operator==(const Int8Array &other) const {
        if (data != other.data) { return false; }
        return true;
    }

    bool operator!=(const Int8Array &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 1;
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