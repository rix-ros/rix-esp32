#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <map>
#include <string>
#include <cstring>

#include "rix/msg/serialization.hpp"
#include "rix/msg/message.hpp"
#include "rix/msg/mediator/ActInfo.hpp"
#include "rix/msg/mediator/NodeInfo.hpp"
#include "rix/msg/mediator/PubInfo.hpp"
#include "rix/msg/mediator/SrvInfo.hpp"
#include "rix/msg/mediator/SubInfo.hpp"

namespace rix {
namespace msg {
namespace mediator {

class SystemInfo : public Message {
  public:
    std::vector<mediator::NodeInfo> nodes{};
    std::vector<mediator::PubInfo> publishers{};
    std::vector<mediator::SubInfo> subscribers{};
    std::vector<mediator::SrvInfo> services{};
    std::vector<mediator::ActInfo> actions{};

    SystemInfo() = default;
    SystemInfo(const SystemInfo &other) = default;
    ~SystemInfo() = default;

    size_t size() const override {
        using namespace detail;
        size_t size = 0;
        size += size_message_vector(nodes);
        size += size_message_vector(publishers);
        size += size_message_vector(subscribers);
        size += size_message_vector(services);
        size += size_message_vector(actions);
        return size;
    }

    std::array<uint64_t, 2> hash() const override {
        return {0x3de740ec767570b8ULL, 0xf9b13b114ccd058bULL};
    }

    void serialize(uint8_t *dst, size_t &offset) const override {
        using namespace detail;
        serialize_message_vector(dst, offset, nodes);
        serialize_message_vector(dst, offset, publishers);
        serialize_message_vector(dst, offset, subscribers);
        serialize_message_vector(dst, offset, services);
        serialize_message_vector(dst, offset, actions);
    }

    bool deserialize(const uint8_t *src, size_t size, size_t &offset) override {
        using namespace detail;
        if (!deserialize_message_vector(nodes, src, size, offset)) { return false; };
        if (!deserialize_message_vector(publishers, src, size, offset)) { return false; };
        if (!deserialize_message_vector(subscribers, src, size, offset)) { return false; };
        if (!deserialize_message_vector(services, src, size, offset)) { return false; };
        if (!deserialize_message_vector(actions, src, size, offset)) { return false; };
        return true;
    }
};

} // namespace mediator
} // namespace msg
} // namespace rix