#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"
#include "rix/sys_msgs/Endpoint.hpp"
#include "rix/sys_msgs/TopicInfo.hpp"

namespace rix {
namespace sys_msgs {

class PubInfo : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    uint64_t id{};
    uint64_t node_id{};
    uint8_t protocol{};
    sys_msgs::TopicInfo topic_info{};
    sys_msgs::Endpoint endpoint{};

    PubInfo() = default;
    PubInfo(const PubInfo &other) = default;
    ~PubInfo() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0xaa95f591adb83351ULL, 0x69dc7c7a949a2d04ULL};
    }

    bool operator==(const PubInfo &other) const {
        if (id != other.id) { return false; }
        if (node_id != other.node_id) { return false; }
        if (protocol != other.protocol) { return false; }
        if (topic_info != other.topic_info) { return false; }
        if (endpoint != other.endpoint) { return false; }
        return true;
    }

    bool operator!=(const PubInfo &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 3;
        count += detail::get_segment_count(this->topic_info);
        count += detail::get_segment_count(this->endpoint);
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->id, segments, offset);
        detail::get_segments(this->node_id, segments, offset);
        detail::get_segments(this->protocol, segments, offset);
        detail::get_segments(this->topic_info, segments, offset);
        detail::get_segments(this->endpoint, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->id, segments, offset);
        detail::get_segments(this->node_id, segments, offset);
        detail::get_segments(this->protocol, segments, offset);
        detail::get_segments(this->topic_info, segments, offset);
        detail::get_segments(this->endpoint, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->topic_info);
        len += detail::get_prefix_len(this->endpoint);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->topic_info, dst, offset);
        detail::get_prefix(this->endpoint, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->topic_info, src, offset);
        detail::resize(this->endpoint, src, offset);
        return true;
    }
};

} // namespace sys_msgs
} // namespace rix