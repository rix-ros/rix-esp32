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

namespace rix {
namespace msg {
namespace mediator {

class ActResponse : public Message {
  public:
    uint8_t error{};
    mediator::ActInfo act_info{};

    ActResponse() = default;
    ActResponse(const ActResponse &other) = default;
    ~ActResponse() = default;

    size_t size() const override {
        using namespace detail;
        size_t size = 0;
        size += size_number(error);
        size += size_message(act_info);
        return size;
    }

    std::array<uint64_t, 2> hash() const override {
        return {0xf50b5c0f46968e3aULL, 0x6b7175cc3a445ad9ULL};
    }

    void serialize(uint8_t *dst, size_t &offset) const override {
        using namespace detail;
        serialize_number(dst, offset, error);
        serialize_message(dst, offset, act_info);
    }

    bool deserialize(const uint8_t *src, size_t size, size_t &offset) override {
        using namespace detail;
        if (!deserialize_number(error, src, size, offset)) { return false; };
        if (!deserialize_message(act_info, src, size, offset)) { return false; };
        return true;
    }
};

} // namespace mediator
} // namespace msg
} // namespace rix