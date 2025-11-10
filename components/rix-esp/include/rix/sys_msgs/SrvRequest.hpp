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

class SrvRequest : public Message {
  public:
    using Message::get_prefix;
    using Message::get_segments;

    uint64_t node_id{};
    std::string name{};
    uint8_t protocol{};
    std::array<uint64_t, 2> request_hash{};
    std::array<uint64_t, 2> response_hash{};

    SrvRequest() = default;
    SrvRequest(const SrvRequest &other) = default;
    ~SrvRequest() = default;

    std::array<uint64_t, 2> hash() const override {
        return {0x8f9b3a9d39fc62f9ULL, 0xcea3e09d7c50ec1eULL};
    }

    bool operator==(const SrvRequest &other) const {
        if (node_id != other.node_id) { return false; }
        if (name != other.name) { return false; }
        if (protocol != other.protocol) { return false; }
        if (request_hash != other.request_hash) { return false; }
        if (response_hash != other.response_hash) { return false; }
        return true;
    }

    bool operator!=(const SrvRequest &other) const {
        return !(*this == other);
    }

    size_t get_segment_count() const override {
        size_t count = 0;
        count += 5;
        return count;
    }

    bool get_segments(MessageSegment *segments, size_t len, size_t &offset) override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->node_id, segments, offset);
        detail::get_segments(this->name, segments, offset);
        detail::get_segments(this->protocol, segments, offset);
        detail::get_segments(this->request_hash, segments, offset);
        detail::get_segments(this->response_hash, segments, offset);
        return true;
    }

    bool get_segments(ConstMessageSegment *segments, size_t len, size_t &offset) const override {
        if (len < offset + get_segment_count()) {
            return false;
        }
        detail::get_segments(this->node_id, segments, offset);
        detail::get_segments(this->name, segments, offset);
        detail::get_segments(this->protocol, segments, offset);
        detail::get_segments(this->request_hash, segments, offset);
        detail::get_segments(this->response_hash, segments, offset);
        return true;
    }

    uint32_t get_prefix_len() const override {
        uint32_t len = 0;
        len += detail::get_prefix_len(this->name);
        return len;
    }

    void get_prefix(uint8_t *dst, size_t &offset) const override {
        detail::get_prefix(this->name, dst, offset);
    }

    bool resize(const uint8_t *src, size_t len, size_t &offset) override {
        if (len < offset + get_prefix_len()) {
            return false;
        }
        detail::resize(this->name, src, offset);
        return true;
    }
};

} // namespace sys_msgs
} // namespace rix