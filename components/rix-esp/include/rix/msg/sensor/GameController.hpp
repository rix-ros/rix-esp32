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

class GameController : public Message {
  public:
    standard::Header header{};
    std::vector<float> axes{};
    std::vector<int32_t> buttons{};

    GameController() = default;
    GameController(const GameController &other) = default;
    ~GameController() = default;

    size_t size() const override {
        using namespace detail;
        size_t size = 0;
        size += size_message(header);
        size += size_number_vector(axes);
        size += size_number_vector(buttons);
        return size;
    }

    std::array<uint64_t, 2> hash() const override {
        return {0x7e1534c7c3f79f57ULL, 0x13a4c5f0b126109eULL};
    }

    void serialize(uint8_t *dst, size_t &offset) const override {
        using namespace detail;
        serialize_message(dst, offset, header);
        serialize_number_vector(dst, offset, axes);
        serialize_number_vector(dst, offset, buttons);
    }

    bool deserialize(const uint8_t *src, size_t size, size_t &offset) override {
        using namespace detail;
        if (!deserialize_message(header, src, size, offset)) { return false; };
        if (!deserialize_number_vector(axes, src, size, offset)) { return false; };
        if (!deserialize_number_vector(buttons, src, size, offset)) { return false; };
        return true;
    }
};

} // namespace sensor
} // namespace msg
} // namespace rix