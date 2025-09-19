#pragma once

#include "rix/ipc/generic_socket.hpp"
#include "rix/ipc/lwip_socket.hpp"
#include <memory>

namespace rix::ipc {
using Socket = rix::ipc::LWIPSocket;

static inline std::shared_ptr<GenericSocket> create_socket() {
  return std::make_shared<Socket>();
}

} // namespace rix::ipc