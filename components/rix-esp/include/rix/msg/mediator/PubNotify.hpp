#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <map>
#include <string>
#include <cstring>

#include "rix/msg/serialization.hpp"
#include "rix/msg/message.hpp"
#include "rix/msg/mediator/SubInfo.hpp"

namespace rix {
namespace msg {
namespace mediator {

class PubNotify : public Message {
  public:
    uint64_t id{};
    bool connect{};
    uint8_t error{};
    std::vector<mediator::SubInfo> subscribers{};

    PubNotify() = default;
    PubNotify(const PubNotify &other) = default;
    ~PubNotify() = default;

    size_t size() const override {
        using namespace detail;
        size_t size = 0;
        size += size_number(id);
        size += size_number(connect);
        size += size_number(error);
        size += size_message_vector(subscribers);
        return size;
    }

    std::array<uint64_t, 2> hash() const override {
        return {0xecd2eab46fd6c2abULL, 0x8c29ce8379c80c39ULL};
    }

    void serialize(uint8_t *dst, size_t &offset) const override {
        using namespace detail;
        serialize_number(dst, offset, id);
        serialize_number(dst, offset, connect);
        serialize_number(dst, offset, error);
        serialize_message_vector(dst, offset, subscribers);
    }

    bool deserialize(const uint8_t *src, size_t size, size_t &offset) override {
        using namespace detail;
        if (!deserialize_number(id, src, size, offset)) { return false; };
        if (!deserialize_number(connect, src, size, offset)) { return false; };
        if (!deserialize_number(error, src, size, offset)) { return false; };
        if (!deserialize_message_vector(subscribers, src, size, offset)) { return false; };
        return true;
    }
};

} // namespace mediator
} // namespace msg
} // namespace rix