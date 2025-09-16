#pragma once

#include "rix/ipc/generic_socket.hpp"
#include "rix/ipc/connection.hpp"
#include <memory>

namespace rix::ipc
{

    class Client : public Connection
    {
    public:
        explicit Client(std::unique_ptr<GenericSocket> socket) : Connection(std::move(socket)) {}
        Client(const Client &) = default;
        Client &operator=(const Client &) = default;
        Client(Client &&) noexcept = default;
        Client &operator=(Client &&) noexcept = default;
        ~Client() = default;

        bool connect(const Endpoint &endpoint) const
        {
            return socket_->connect(endpoint);
        }

        bool is_connected() const { return wait_connected(rix::util::Duration(0)); }

        bool wait_connected(const rix::util::Duration &timeout) const
        {
            return socket_->wait_writable(timeout);
        }
    };

}