#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <map>
#include <string>
#include <cstring>

#include "rix/msg/serialization.hpp"
#include "rix/msg/message.hpp"

namespace rix {
namespace msg {
namespace mediator {

class SrvRequest : public Message {
  public:
    uint64_t node_id{};
    std::string name{};
    uint8_t protocol{};
    std::array<uint64_t, 2> request_hash{};
    std::array<uint64_t, 2> response_hash{};

    SrvRequest() = default;
    SrvRequest(const SrvRequest &other) = default;
    ~SrvRequest() = default;

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

    size_t size() const override {
        using namespace detail;
        size_t size = 0;
        size += size_number(node_id);
        size += size_string(name);
        size += size_number(protocol);
        size += size_number_array(request_hash);
        size += size_number_array(response_hash);
        return size;
    }

    std::array<uint64_t, 2> hash() const override {
        return {0x83c00ab87473b94dULL, 0xe7eb7f444d726b66ULL};
    }

    void serialize(uint8_t *dst, size_t &offset) const override {
        using namespace detail;
        serialize_number(dst, offset, node_id);
        serialize_string(dst, offset, name);
        serialize_number(dst, offset, protocol);
        serialize_number_array(dst, offset, request_hash);
        serialize_number_array(dst, offset, response_hash);
    }

    bool deserialize(const uint8_t *src, size_t size, size_t &offset) override {
        using namespace detail;
        if (!deserialize_number(node_id, src, size, offset)) { return false; };
        if (!deserialize_string(name, src, size, offset)) { return false; };
        if (!deserialize_number(protocol, src, size, offset)) { return false; };
        if (!deserialize_number_array(request_hash, src, size, offset)) { return false; };
        if (!deserialize_number_array(response_hash, src, size, offset)) { return false; };
        return true;
    }
};

} // namespace mediator
} // namespace msg
} // namespace rix