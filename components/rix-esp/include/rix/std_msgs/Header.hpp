#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message.hpp"
#include "rix/msg/serialization.hpp"
#include "rix/std_msgs/Time.hpp"

namespace rix {
namespace std_msgs {

class Header : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    uint32_t seq{};
    std_msgs::Time stamp{};
    std::string frame_id{};

    Header() = default;
    Header(const Header &other) = default;
    ~Header() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0xe3a6ed6f693f7ed6ULL, 0xa35aa20906befa62ULL};
    }

    bool operator==(const Header &other) const {
        if (seq != other.seq) { return false; }
        if (stamp != other.stamp) { return false; }
        if (frame_id != other.frame_id) { return false; }
        return true;
    }

    bool operator!=(const Header &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 2;
        count += detail::get_segment_count(this->stamp);
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->seq, segments, offset);
        detail::get_segments(this->stamp, segments, offset);
        detail::get_segments(this->frame_id, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->seq, segments, offset);
        detail::get_segments(this->stamp, segments, offset);
        detail::get_segments(this->frame_id, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->stamp);
        len += detail::get_prefix_len(this->frame_id);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->stamp, dst, offset);
        detail::get_prefix(this->frame_id, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->stamp, src, offset);
        detail::resize(this->frame_id, src, offset);
        return true;
    }
};

} // namespace std_msgs
} // namespace rix