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
namespace sensor {

class ChannelInt32 : public Message {
  public:
    std::string name{};
    std::vector<int32_t> data{};

    ChannelInt32() = default;
    ChannelInt32(const ChannelInt32 &other) = default;
    ~ChannelInt32() = default;

    bool operator==(const ChannelInt32 &other) const {
        if (name != other.name) { return false; }
        if (data != other.data) { return false; }
        return true;
    }

    bool operator!=(const ChannelInt32 &other) const {
        return !(*this == other);
    }

    size_t size() const override {
        using namespace detail;
        size_t size = 0;
        size += size_string(name);
        size += size_number_vector(data);
        return size;
    }

    std::array<uint64_t, 2> hash() const override {
        return {0x1fe068ef0a63a5a2ULL, 0x8982aea98e0d5f22ULL};
    }

    void serialize(uint8_t *dst, size_t &offset) const override {
        using namespace detail;
        serialize_string(dst, offset, name);
        serialize_number_vector(dst, offset, data);
    }

    bool deserialize(const uint8_t *src, size_t size, size_t &offset) override {
        using namespace detail;
        if (!deserialize_string(name, src, size, offset)) { return false; };
        if (!deserialize_number_vector(data, src, size, offset)) { return false; };
        return true;
    }
};

} // namespace sensor
} // namespace msg
} // namespace rix