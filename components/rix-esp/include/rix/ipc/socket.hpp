#pragma once

#include "rix/ipc/generic_socket.hpp"
#include <memory>
#include "rix/ipc/lwip_socket.hpp"
#include "rix/ipc/poll.hpp"

namespace rix {

using Socket = rix::ipc::LWIPSocket;
static inline std::shared_ptr<GenericSocket> create_socket() { return std::make_shared<Socket>(); }

} // namespace rix