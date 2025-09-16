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

class ActRequest : public Message {
  public:
    uint64_t node_id{};
    std::string name{};
    uint8_t protocol{};
    std::array<uint64_t, 2> goal_hash{};
    std::array<uint64_t, 2> feedback_hash{};
    std::array<uint64_t, 2> result_hash{};

    ActRequest() = default;
    ActRequest(const ActRequest &other) = default;
    ~ActRequest() = default;

    size_t size() const override {
        using namespace detail;
        size_t size = 0;
        size += size_number(node_id);
        size += size_string(name);
        size += size_number(protocol);
        size += size_number_array(goal_hash);
        size += size_number_array(feedback_hash);
        size += size_number_array(result_hash);
        return size;
    }

    std::array<uint64_t, 2> hash() const override {
        return {0x7348c15675951dc2ULL, 0xfbb1e7d5443584e2ULL};
    }

    void serialize(uint8_t *dst, size_t &offset) const override {
        using namespace detail;
        serialize_number(dst, offset, node_id);
        serialize_string(dst, offset, name);
        serialize_number(dst, offset, protocol);
        serialize_number_array(dst, offset, goal_hash);
        serialize_number_array(dst, offset, feedback_hash);
        serialize_number_array(dst, offset, result_hash);
    }

    bool deserialize(const uint8_t *src, size_t size, size_t &offset) override {
        using namespace detail;
        if (!deserialize_number(node_id, src, size, offset)) { return false; };
        if (!deserialize_string(name, src, size, offset)) { return false; };
        if (!deserialize_number(protocol, src, size, offset)) { return false; };
        if (!deserialize_number_array(goal_hash, src, size, offset)) { return false; };
        if (!deserialize_number_array(feedback_hash, src, size, offset)) { return false; };
        if (!deserialize_number_array(result_hash, src, size, offset)) { return false; };
        return true;
    }
};

} // namespace mediator
} // namespace msg
} // namespace rix