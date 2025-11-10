#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"
#include "rix/sys_msgs/SrvInfo.hpp"

namespace rix {
namespace sys_msgs {

class SrvResponse : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    uint8_t error{};
    sys_msgs::SrvInfo srv_info{};

    SrvResponse() = default;
    SrvResponse(const SrvResponse &other) = default;
    ~SrvResponse() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0x9494f719aba9c47fULL, 0xed9f89fd6405040cULL};
    }

    bool operator==(const SrvResponse &other) const {
        if (error != other.error) { return false; }
        if (srv_info != other.srv_info) { return false; }
        return true;
    }

    bool operator!=(const SrvResponse &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 1;
        count += detail::get_segment_count(this->srv_info);
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->error, segments, offset);
        detail::get_segments(this->srv_info, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->error, segments, offset);
        detail::get_segments(this->srv_info, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->srv_info);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->srv_info, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->srv_info, src, offset);
        return true;
    }
};

} // namespace sys_msgs
} // namespace rix