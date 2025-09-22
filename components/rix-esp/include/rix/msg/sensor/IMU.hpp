#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <map>
#include <string>
#include <cstring>

#include "rix/msg/serialization.hpp"
#include "rix/msg/message.hpp"
#include "rix/msg/geometry/Quaternion.hpp"
#include "rix/msg/geometry/Vector3.hpp"
#include "rix/msg/standard/Header.hpp"

namespace rix {
namespace msg {
namespace sensor {

class IMU : public Message {
  public:
    standard::Header header{};
    geometry::Quaternion orientation{};
    geometry::Vector3 angular_velocity{};
    geometry::Vector3 linear_acceleration{};

    IMU() = default;
    IMU(const IMU &other) = default;
    ~IMU() = default;

    size_t size() const override {
        using namespace detail;
        size_t size = 0;
        size += size_message(header);
        size += size_message(orientation);
        size += size_message(angular_velocity);
        size += size_message(linear_acceleration);
        return size;
    }

    std::array<uint64_t, 2> hash() const override {
        return {0xea0a73fa393c53c3ULL, 0x5e980c6ed199f2e5ULL};
    }

    void serialize(uint8_t *dst, size_t &offset) const override {
        using namespace detail;
        serialize_message(dst, offset, header);
        serialize_message(dst, offset, orientation);
        serialize_message(dst, offset, angular_velocity);
        serialize_message(dst, offset, linear_acceleration);
    }

    bool deserialize(const uint8_t *src, size_t size, size_t &offset) override {
        using namespace detail;
        if (!deserialize_message(header, src, size, offset)) { return false; };
        if (!deserialize_message(orientation, src, size, offset)) { return false; };
        if (!deserialize_message(angular_velocity, src, size, offset)) { return false; };
        if (!deserialize_message(linear_acceleration, src, size, offset)) { return false; };
        return true;
    }
};

} // namespace sensor
} // namespace msg
} // namespace rix