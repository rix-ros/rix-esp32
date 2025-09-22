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

class WheelEncoder : public Message {
  public:
    standard::Header header{};
    std::vector<int64_t> ticks{};
    std::vector<int32_t> delta_ticks{};
    int32_t delta_time{};

    WheelEncoder() = default;
    WheelEncoder(const WheelEncoder &other) = default;
    ~WheelEncoder() = default;

    size_t size() const override {
        using namespace detail;
        size_t size = 0;
        size += size_message(header);
        size += size_number_vector(ticks);
        size += size_number_vector(delta_ticks);
        size += size_number(delta_time);
        return size;
    }

    std::array<uint64_t, 2> hash() const override {
        return {0xb1bb3ab289b9324aULL, 0x8e71f13d72e4a8aeULL};
    }

    void serialize(uint8_t *dst, size_t &offset) const override {
        using namespace detail;
        serialize_message(dst, offset, header);
        serialize_number_vector(dst, offset, ticks);
        serialize_number_vector(dst, offset, delta_ticks);
        serialize_number(dst, offset, delta_time);
    }

    bool deserialize(const uint8_t *src, size_t size, size_t &offset) override {
        using namespace detail;
        if (!deserialize_message(header, src, size, offset)) { return false; };
        if (!deserialize_number_vector(ticks, src, size, offset)) { return false; };
        if (!deserialize_number_vector(delta_ticks, src, size, offset)) { return false; };
        if (!deserialize_number(delta_time, src, size, offset)) { return false; };
        return true;
    }
};

} // namespace sensor
} // namespace msg
} // namespace rix