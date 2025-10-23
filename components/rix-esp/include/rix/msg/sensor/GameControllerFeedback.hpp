#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <map>
#include <string>
#include <cstring>

#include "rix/msg/serialization.hpp"
#include "rix/msg/message.hpp"
#include "rix/msg/standard/Duration.hpp"
#include "rix/msg/standard/Header.hpp"

namespace rix {
namespace msg {
namespace sensor {

class GameControllerFeedback : public Message {
  public:
    standard::Header header{};
    std::vector<float> intensities{};
    std::vector<standard::Duration> durations{};

    GameControllerFeedback() = default;
    GameControllerFeedback(const GameControllerFeedback &other) = default;
    ~GameControllerFeedback() = default;

    size_t size() const override {
        using namespace detail;
        size_t size = 0;
        size += size_message(header);
        size += size_number_vector(intensities);
        size += size_message_vector(durations);
        return size;
    }

    std::array<uint64_t, 2> hash() const override {
        return {0x179e24083020f533ULL, 0x5b33b70a8fc2cd33ULL};
    }

    void serialize(uint8_t *dst, size_t &offset) const override {
        using namespace detail;
        serialize_message(dst, offset, header);
        serialize_number_vector(dst, offset, intensities);
        serialize_message_vector(dst, offset, durations);
    }

    bool deserialize(const uint8_t *src, size_t size, size_t &offset) override {
        using namespace detail;
        if (!deserialize_message(header, src, size, offset)) { return false; };
        if (!deserialize_number_vector(intensities, src, size, offset)) { return false; };
        if (!deserialize_message_vector(durations, src, size, offset)) { return false; };
        return true;
    }
};

} // namespace sensor
} // namespace msg
} // namespace rix