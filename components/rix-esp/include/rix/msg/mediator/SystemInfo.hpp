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
#include "rix/msg/mediator/TopicInfo.hpp"

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
    std::vector<mediator::TopicInfo> topics{};

    SystemInfo() = default;
    SystemInfo(const SystemInfo &other) = default;
    ~SystemInfo() = default;

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

    size_t size() const override {
        using namespace detail;
        size_t size = 0;
        size += size_message_vector(nodes);
        size += size_message_vector(publishers);
        size += size_message_vector(subscribers);
        size += size_message_vector(services);
        size += size_message_vector(actions);
        size += size_message_vector(topics);
        return size;
    }

    std::array<uint64_t, 2> hash() const override {
        return {0x3f4632e08eb50ad6ULL, 0x4b9447c0f29dceb6ULL};
    }

    void serialize(uint8_t *dst, size_t &offset) const override {
        using namespace detail;
        serialize_message_vector(dst, offset, nodes);
        serialize_message_vector(dst, offset, publishers);
        serialize_message_vector(dst, offset, subscribers);
        serialize_message_vector(dst, offset, services);
        serialize_message_vector(dst, offset, actions);
        serialize_message_vector(dst, offset, topics);
    }

    bool deserialize(const uint8_t *src, size_t size, size_t &offset) override {
        using namespace detail;
        if (!deserialize_message_vector(nodes, src, size, offset)) { return false; };
        if (!deserialize_message_vector(publishers, src, size, offset)) { return false; };
        if (!deserialize_message_vector(subscribers, src, size, offset)) { return false; };
        if (!deserialize_message_vector(services, src, size, offset)) { return false; };
        if (!deserialize_message_vector(actions, src, size, offset)) { return false; };
        if (!deserialize_message_vector(topics, src, size, offset)) { return false; };
        return true;
    }
};

} // namespace mediator
} // namespace msg
} // namespace rix