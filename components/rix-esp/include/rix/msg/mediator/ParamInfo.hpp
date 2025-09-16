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
namespace mediator {

class ParamInfo : public Message {
  public:
    std::string name{};
    std::array<uint64_t, 2> message_hash{};
    std::vector<uint8_t> data{};

    ParamInfo() = default;
    ParamInfo(const ParamInfo &other) = default;
    ~ParamInfo() = default;

    size_t size() const override {
        using namespace detail;
        size_t size = 0;
        size += size_string(name);
        size += size_number_array(message_hash);
        size += size_number_vector(data);
        return size;
    }

    std::array<uint64_t, 2> hash() const override {
        return {0x9bf7c2ee524c2032ULL, 0x5e66e007fdac35eeULL};
    }

    void serialize(uint8_t *dst, size_t &offset) const override {
        using namespace detail;
        serialize_string(dst, offset, name);
        serialize_number_array(dst, offset, message_hash);
        serialize_number_vector(dst, offset, data);
    }

    bool deserialize(const uint8_t *src, size_t size, size_t &offset) override {
        using namespace detail;
        if (!deserialize_string(name, src, size, offset)) { return false; };
        if (!deserialize_number_array(message_hash, src, size, offset)) { return false; };
        if (!deserialize_number_vector(data, src, size, offset)) { return false; };
        return true;
    }
};

} // namespace mediator
} // namespace msg
} // namespace rix