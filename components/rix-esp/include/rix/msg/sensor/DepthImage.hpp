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

class DepthImage : public Message {
  public:
    standard::Header header{};
    uint32_t width{};
    uint32_t height{};
    std::vector<float> data{};

    DepthImage() = default;
    DepthImage(const DepthImage &other) = default;
    ~DepthImage() = default;

    bool operator==(const DepthImage &other) const {
        if (header != other.header) { return false; }
        if (width != other.width) { return false; }
        if (height != other.height) { return false; }
        if (data != other.data) { return false; }
        return true;
    }

    bool operator!=(const DepthImage &other) const {
        return !(*this == other);
    }

    size_t size() const override {
        using namespace detail;
        size_t size = 0;
        size += size_message(header);
        size += size_number(width);
        size += size_number(height);
        size += size_number_vector(data);
        return size;
    }

    std::array<uint64_t, 2> hash() const override {
        return {0x7b679082dc7b2b53ULL, 0x0db3b8881837cd7eULL};
    }

    void serialize(uint8_t *dst, size_t &offset) const override {
        using namespace detail;
        serialize_message(dst, offset, header);
        serialize_number(dst, offset, width);
        serialize_number(dst, offset, height);
        serialize_number_vector(dst, offset, data);
    }

    bool deserialize(const uint8_t *src, size_t size, size_t &offset) override {
        using namespace detail;
        if (!deserialize_message(header, src, size, offset)) { return false; };
        if (!deserialize_number(width, src, size, offset)) { return false; };
        if (!deserialize_number(height, src, size, offset)) { return false; };
        if (!deserialize_number_vector(data, src, size, offset)) { return false; };
        return true;
    }
};

} // namespace sensor
} // namespace msg
} // namespace rix