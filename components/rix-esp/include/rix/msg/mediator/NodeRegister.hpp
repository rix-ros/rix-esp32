#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <cstring>

#include "rix/msg/message_base.hpp"
#include "rix/msg/mediator/Endpoint.hpp"

namespace rix {
namespace msg {
namespace mediator {

class NodeRegister : public MessageBase {
  public:
    uint64_t machine_id;
    uint64_t id;
    std::string name;
    mediator::Endpoint endpoint;

    NodeRegister() = default;
    NodeRegister(const NodeRegister &other) = default;
    ~NodeRegister() = default;

    size_t size() const override {
        using namespace detail;
        size_t size = 0;
        size += size_base(machine_id);
        size += size_base(id);
        size += size_string(name);
        size += size_custom(endpoint);
        return size;
    }

    std::array<uint64_t, 2> hash() const override {
        return {0xf4242241b6d39059ULL, 0xf32f049f07439335ULL};
    }

    bool serialize(std::vector<uint8_t> &buffer) const override {
        using namespace detail;
        if (buffer.size() + this->size() > buffer.max_size()) {
            return false;
        }
        buffer.reserve(buffer.size() + this->size());
        serialize_base(machine_id, buffer);
        serialize_base(id, buffer);
        serialize_string(name, buffer);
        serialize_custom(endpoint, buffer);
        return true;
    }

    bool deserialize(const std::vector<uint8_t> &buffer, size_t &offset) override {
        using namespace detail;
        if (!deserialize_base(machine_id, buffer, offset)) { return false; };
        if (!deserialize_base(id, buffer, offset)) { return false; };
        if (!deserialize_string(name, buffer, offset)) { return false; };
        if (!deserialize_custom(endpoint, buffer, offset)) { return false; };
        return true;
    }
};

} // namespace mediator
} // namespace msg
} // namespace rix