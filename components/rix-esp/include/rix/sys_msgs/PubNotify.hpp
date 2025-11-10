#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"
#include "rix/sys_msgs/SubInfo.hpp"

namespace rix {
namespace sys_msgs {

class PubNotify : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    uint64_t id{};
    bool connect{};
    uint8_t error{};
    std::vector<sys_msgs::SubInfo> subscribers{};

    PubNotify() = default;
    PubNotify(const PubNotify &other) = default;
    ~PubNotify() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0x03bf829073ac126fULL, 0xeebd6c704bf6aff2ULL};
    }

    bool operator==(const PubNotify &other) const {
        if (id != other.id) { return false; }
        if (connect != other.connect) { return false; }
        if (error != other.error) { return false; }
        if (subscribers != other.subscribers) { return false; }
        return true;
    }

    bool operator!=(const PubNotify &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 3;
        count += detail::get_segment_count(this->subscribers);
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->id, segments, offset);
        detail::get_segments(this->connect, segments, offset);
        detail::get_segments(this->error, segments, offset);
        detail::get_segments(this->subscribers, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->id, segments, offset);
        detail::get_segments(this->connect, segments, offset);
        detail::get_segments(this->error, segments, offset);
        detail::get_segments(this->subscribers, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->subscribers);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->subscribers, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->subscribers, src, offset);
        return true;
    }
};

} // namespace sys_msgs
} // namespace rix