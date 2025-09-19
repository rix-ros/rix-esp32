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
namespace standard {

class Void : public Message {
  public:
    

    Void() = default;
    Void(const Void &other) = default;
    ~Void() = default;

    size_t size() const override {
        using namespace detail;
        size_t size = 0;
        
        return size;
    }

    std::array<uint64_t, 2> hash() const override {
        return {0xb325893ac89b970eULL, 0x1ba2692e33512f46ULL};
    }

    void serialize(uint8_t *dst, size_t &offset) const override {
        using namespace detail;
        
    }

    bool deserialize(const uint8_t *src, size_t size, size_t &offset) override {
        using namespace detail;
        
        return true;
    }
};

} // namespace standard
} // namespace msg
} // namespace rix