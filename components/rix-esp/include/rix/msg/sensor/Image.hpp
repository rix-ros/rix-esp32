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

class Image : public Message {
  public:
    standard::Header header{};
    uint32_t width{};
    uint32_t height{};
    uint32_t channels{};
    std::vector<uint8_t> data{};

    Image() = default;
    Image(const Image &other) = default;
    ~Image() = default;

    size_t size() const override {
        using namespace detail;
        size_t size = 0;
        size += size_message(header);
        size += size_number(width);
        size += size_number(height);
        size += size_number(channels);
        size += size_number_vector(data);
        return size;
    }

    std::array<uint64_t, 2> hash() const override {
        return {0xeb830a707853bcfbULL, 0x532a0ab53aac7364ULL};
    }

    void serialize(uint8_t *dst, size_t &offset) const override {
        using namespace detail;
        serialize_message(dst, offset, header);
        serialize_number(dst, offset, width);
        serialize_number(dst, offset, height);
        serialize_number(dst, offset, channels);
        serialize_number_vector(dst, offset, data);
    }

    bool deserialize(const uint8_t *src, size_t size, size_t &offset) override {
        using namespace detail;
        if (!deserialize_message(header, src, size, offset)) { return false; };
        if (!deserialize_number(width, src, size, offset)) { return false; };
        if (!deserialize_number(height, src, size, offset)) { return false; };
        if (!deserialize_number(channels, src, size, offset)) { return false; };
        if (!deserialize_number_vector(data, src, size, offset)) { return false; };
        return true;
    }
};

} // namespace sensor
} // namespace msg
} // namespace rix