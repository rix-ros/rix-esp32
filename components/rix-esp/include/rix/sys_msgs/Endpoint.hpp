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

class Endpoint : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    uint16_t port{};
    std::string address{};

    Endpoint() = default;
    Endpoint(const Endpoint &other) = default;
    ~Endpoint() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0x16aad12ed93a7197ULL, 0x0b637efe671537faULL};
    }

    bool operator==(const Endpoint &other) const {
        if (port != other.port) { return false; }
        if (address != other.address) { return false; }
        return true;
    }

    bool operator!=(const Endpoint &other) const {
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
        detail::get_segments(this->port, segments, offset);
        detail::get_segments(this->address, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->port, segments, offset);
        detail::get_segments(this->address, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->address);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->address, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->address, src, offset);
        return true;
    }
};

} // namespace sys_msgs
} // namespace rix