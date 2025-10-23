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

class CompressedImage : public Message {
  public:
    standard::Header header{};
    std::vector<uint8_t> data{};

    CompressedImage() = default;
    CompressedImage(const CompressedImage &other) = default;
    ~CompressedImage() = default;

    bool operator==(const CompressedImage &other) const {
        if (header != other.header) { return false; }
        if (data != other.data) { return false; }
        return true;
    }

    bool operator!=(const CompressedImage &other) const {
        return !(*this == other);
    }

    size_t size() const override {
        using namespace detail;
        size_t size = 0;
        size += size_message(header);
        size += size_number_vector(data);
        return size;
    }

    std::array<uint64_t, 2> hash() const override {
        return {0xcd18494cd9a05543ULL, 0xb8b8bc270304ba83ULL};
    }

    void serialize(uint8_t *dst, size_t &offset) const override {
        using namespace detail;
        serialize_message(dst, offset, header);
        serialize_number_vector(dst, offset, data);
    }

    bool deserialize(const uint8_t *src, size_t size, size_t &offset) override {
        using namespace detail;
        if (!deserialize_message(header, src, size, offset)) { return false; };
        if (!deserialize_number_vector(data, src, size, offset)) { return false; };
        return true;
    }
};

} // namespace sensor
} // namespace msg
} // namespace rix