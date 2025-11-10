#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"
#include "rix/sys_msgs/Endpoint.hpp"

namespace rix {
namespace sys_msgs {

class NodeInfo : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    std::string name{};
    uint64_t id{};
    uint64_t machine_id{};
    uint8_t protocol{};
    sys_msgs::Endpoint endpoint{};

    NodeInfo() = default;
    NodeInfo(const NodeInfo &other) = default;
    ~NodeInfo() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0xb801f3a64056a15fULL, 0xf05107c433b4d19bULL};
    }

    bool operator==(const NodeInfo &other) const {
        if (name != other.name) { return false; }
        if (id != other.id) { return false; }
        if (machine_id != other.machine_id) { return false; }
        if (protocol != other.protocol) { return false; }
        if (endpoint != other.endpoint) { return false; }
        return true;
    }

    bool operator!=(const NodeInfo &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 4;
        count += detail::get_segment_count(this->endpoint);
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->name, segments, offset);
        detail::get_segments(this->id, segments, offset);
        detail::get_segments(this->machine_id, segments, offset);
        detail::get_segments(this->protocol, segments, offset);
        detail::get_segments(this->endpoint, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->name, segments, offset);
        detail::get_segments(this->id, segments, offset);
        detail::get_segments(this->machine_id, segments, offset);
        detail::get_segments(this->protocol, segments, offset);
        detail::get_segments(this->endpoint, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->name);
        len += detail::get_prefix_len(this->endpoint);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->name, dst, offset);
        detail::get_prefix(this->endpoint, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->name, src, offset);
        detail::resize(this->endpoint, src, offset);
        return true;
    }
};

} // namespace sys_msgs
} // namespace rix