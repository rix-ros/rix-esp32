#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <map>
#include <string>
#include <cstring>

#include "rix/msg/serialization.hpp"
#include "rix/msg/message.hpp"
#include "rix/msg/standard/Header.hpp"

namespace rix {
namespace msg {
namespace sensor {

class MotorVelocity : public Message {
  public:
    standard::Header header{};
    std::vector<float> velocity{};

    MotorVelocity() = default;
    MotorVelocity(const MotorVelocity &other) = default;
    ~MotorVelocity() = default;

    size_t size() const override {
        using namespace detail;
        size_t size = 0;
        size += size_message(header);
        size += size_number_vector(velocity);
        return size;
    }

    std::array<uint64_t, 2> hash() const override {
        return {0x013ff91a89c54064ULL, 0xd095f9402deba652ULL};
    }

    void serialize(uint8_t *dst, size_t &offset) const override {
        using namespace detail;
        serialize_message(dst, offset, header);
        serialize_number_vector(dst, offset, velocity);
    }

    bool deserialize(const uint8_t *src, size_t size, size_t &offset) override {
        using namespace detail;
        if (!deserialize_message(header, src, size, offset)) { return false; };
        if (!deserialize_number_vector(velocity, src, size, offset)) { return false; };
        return true;
    }
};

} // namespace sensor
} // namespace msg
} // namespace rix