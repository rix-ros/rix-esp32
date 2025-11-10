#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"
#include "rix/sys_msgs/ActInfo.hpp"
#include "rix/sys_msgs/NodeInfo.hpp"
#include "rix/sys_msgs/PubInfo.hpp"
#include "rix/sys_msgs/SrvInfo.hpp"
#include "rix/sys_msgs/SubInfo.hpp"
#include "rix/sys_msgs/TopicInfo.hpp"

namespace rix {
namespace sys_msgs {

class SystemInfo : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    std::vector<sys_msgs::NodeInfo> nodes{};
    std::vector<sys_msgs::PubInfo> publishers{};
    std::vector<sys_msgs::SubInfo> subscribers{};
    std::vector<sys_msgs::SrvInfo> services{};
    std::vector<sys_msgs::ActInfo> actions{};
    std::vector<sys_msgs::TopicInfo> topics{};

    SystemInfo() = default;
    SystemInfo(const SystemInfo &other) = default;
    ~SystemInfo() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0x04893a9836383fdeULL, 0xa9d609ecf97167e5ULL};
    }

    bool operator==(const SystemInfo &other) const {
        if (nodes != other.nodes) { return false; }
        if (publishers != other.publishers) { return false; }
        if (subscribers != other.subscribers) { return false; }
        if (services != other.services) { return false; }
        if (actions != other.actions) { return false; }
        if (topics != other.topics) { return false; }
        return true;
    }

    bool operator!=(const SystemInfo &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 0;
        count += detail::get_segment_count(this->nodes);
        count += detail::get_segment_count(this->publishers);
        count += detail::get_segment_count(this->subscribers);
        count += detail::get_segment_count(this->services);
        count += detail::get_segment_count(this->actions);
        count += detail::get_segment_count(this->topics);
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->nodes, segments, offset);
        detail::get_segments(this->publishers, segments, offset);
        detail::get_segments(this->subscribers, segments, offset);
        detail::get_segments(this->services, segments, offset);
        detail::get_segments(this->actions, segments, offset);
        detail::get_segments(this->topics, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->nodes, segments, offset);
        detail::get_segments(this->publishers, segments, offset);
        detail::get_segments(this->subscribers, segments, offset);
        detail::get_segments(this->services, segments, offset);
        detail::get_segments(this->actions, segments, offset);
        detail::get_segments(this->topics, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->nodes);
        len += detail::get_prefix_len(this->publishers);
        len += detail::get_prefix_len(this->subscribers);
        len += detail::get_prefix_len(this->services);
        len += detail::get_prefix_len(this->actions);
        len += detail::get_prefix_len(this->topics);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->nodes, dst, offset);
        detail::get_prefix(this->publishers, dst, offset);
        detail::get_prefix(this->subscribers, dst, offset);
        detail::get_prefix(this->services, dst, offset);
        detail::get_prefix(this->actions, dst, offset);
        detail::get_prefix(this->topics, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->nodes, src, offset);
        detail::resize(this->publishers, src, offset);
        detail::resize(this->subscribers, src, offset);
        detail::resize(this->services, src, offset);
        detail::resize(this->actions, src, offset);
        detail::resize(this->topics, src, offset);
        return true;
    }
};

} // namespace sys_msgs
} // namespace rix