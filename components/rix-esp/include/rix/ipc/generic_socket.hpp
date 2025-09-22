#pragma once

#include "rix/ipc/endpoint.hpp"
#include "rix/msg/mediator/Operation.hpp"
#include "rix/msg/message.hpp"
#include "rix/util/time.hpp"

#include <memory>
namespace rix::ipc {

const int MAX_CONN = 128;

class GenericSocket {
public:
  template<typename Iterator, typename T = typename std::iterator_traits<Iterator>::value_type>
  static std::vector<T> poll(Iterator first, Iterator last, int flags) {
    // Assert that T is a GenericSocket pointer (raw or shared, not unique)
    using PointeeType = typename std::conditional<
        std::is_pointer<T>::value,
        typename std::remove_pointer<T>::type,
        typename T::element_type
    >::type;
    static_assert(std::is_base_of<rix::ipc::GenericSocket, PointeeType>::value, 
                  "T must be a pointer to a subclass of rix::ipc::GenericSocket.");
    
    // Poll flags: 0x1 = POLLIN (readable), 0x4 = POLLOUT (writable), 0x8 = POLLERR (error)
    // Usage: auto ready_sockets = GenericSocket::poll(sockets.begin(), sockets.end(), 0x1);
    
    std::vector<T> socketList;
    fd_set readFds, writeFds, errorFds;
    FD_ZERO(&readFds);
    FD_ZERO(&writeFds);
    FD_ZERO(&errorFds);
    int maxFd = -1;
    
    // Populate fd_set and store socket pointers for later reference
    socketList.reserve(std::distance(first, last));
    for (Iterator it = first; it != last; ++it) {
      auto ptr = *it;
      int fd = ptr->get_fd();
      socketList.push_back(ptr);
      
      // Set appropriate fd_sets based on flags
      if (flags & 0x1) FD_SET(fd, &readFds);   // POLLIN-like
      if (flags & 0x4) FD_SET(fd, &writeFds);  // POLLOUT-like
      if (flags & 0x8) FD_SET(fd, &errorFds);  // POLLERR-like
      
      if (fd > maxFd) {
        maxFd = fd;
      }
    }
    
    // Perform select operation (block indefinitely for now)
    int result = select(maxFd + 1, 
                       (flags & 0x1) ? &readFds : nullptr,
                       (flags & 0x4) ? &writeFds : nullptr, 
                       (flags & 0x8) ? &errorFds : nullptr,
                       nullptr);
    
    // Return vector of GenericSocket pointers that are ready
    std::vector<T> readySockets;
    if (result > 0) {
      for (size_t i = 0; i < socketList.size(); ++i) {
        auto socket = socketList[i];
        int fd = socket->get_fd();
        bool isReady = false;
        
        if ((flags & 0x1) && FD_ISSET(fd, &readFds)) isReady = true;
        if ((flags & 0x4) && FD_ISSET(fd, &writeFds)) isReady = true;
        if ((flags & 0x8) && FD_ISSET(fd, &errorFds)) isReady = true;
        
        if (isReady) {
          readySockets.push_back(socket);
        }
      }
    }
    
    return readySockets;
  }

  // Constructor and Destructor
  GenericSocket() = default;
  virtual ~GenericSocket() = default;

  // Disable copy and move semantics (force use of shared/unique pointers)
  GenericSocket(const GenericSocket &) = delete;
  GenericSocket &operator=(const GenericSocket &) = delete;
  GenericSocket(GenericSocket &&) = delete;
  GenericSocket &operator=(GenericSocket &&) = delete;

  // Socket state operations
  virtual bool bind(const Endpoint &endpoint) const = 0;
  virtual bool listen(int backlog) const = 0;
  virtual std::shared_ptr<GenericSocket> accept(Endpoint &remote_endpoint) const = 0;
  std::shared_ptr<GenericSocket> accept() const {
    Endpoint ep;
    return accept(ep);
  }
  virtual bool connect(const Endpoint &endpoint) const = 0;
  virtual void close() const = 0;

  // I/O multiplexing operations
  virtual bool wait_readable(const rix::util::Duration &timeout) const = 0;
  virtual bool wait_writable(const rix::util::Duration &timeout) const = 0;
  virtual bool wait_exception(const rix::util::Duration &timeout) const = 0;

  // Socket control operations
  virtual bool set_blocking(bool blocking) const = 0;
  virtual bool get_blocking() const = 0;
  virtual bool set_reuse_address(bool reuse) const = 0;
  virtual bool get_reuse_address() const = 0;

  // Endpoint retrieval
  virtual Endpoint local_endpoint() const = 0;
  virtual Endpoint remote_endpoint() const = 0;

  // Get file descriptor
  virtual int get_fd() const = 0;

  bool is_writable() const { return wait_writable(rix::util::Duration(0.0)); }
  bool is_readable() const { return wait_readable(rix::util::Duration(0.0)); }
  bool is_exception() const { return wait_exception(rix::util::Duration(0.0)); }

  // Write operation and message
  virtual bool send_message(uint8_t opcode, const rix::msg::Message &msg) const {
    // Serialize the message
    rix::msg::mediator::Operation op;
    op.len = msg.size();
    op.opcode = opcode;
    std::vector<uint8_t> buffer(op.size() + msg.size());
    size_t offset = 0;
    op.serialize(buffer.data(), offset);
    msg.serialize(buffer.data(), offset);

    ssize_t bytes = 0;
    while (bytes < buffer.size()) {
      ssize_t result = send(buffer.data() + bytes, buffer.size() - bytes, 0);
      if (result <= 0) {
        return false;
      }
      bytes += result;
    }
    return bytes == buffer.size();
  }

  // Read message only
  virtual bool recv_message(rix::msg::Message &msg, size_t len) const {
    // Read the message body only
    std::vector<uint8_t> buffer(len);
    ssize_t bytes = 0;
    while (bytes < buffer.size()) {
      ssize_t result = recv(buffer.data() + bytes, buffer.size() - bytes, 0);
      if (result <= 0) {
        return false;
      }
      bytes += result;
    }
    size_t offset = 0;
    if (!msg.deserialize(buffer.data(), buffer.size(), offset)) {
      return false;
    }
    return true;
  }

  // Read both operation and message (useful if message type is known)
  bool recv_message(rix::msg::mediator::Operation &op, rix::msg::Message &msg) const {
    // Read the operation header first
    if (!recv_message(op, op.size())) {
      return false;
    }
    // Then read the message body
    if (!recv_message(msg, op.len)) {
      return false;
    }
    return true;
  }

private:
  // Low-level I/O operations to be implemented by derived classes
  virtual ssize_t send(const void *buf, size_t len, int flags) const = 0;
  virtual ssize_t recv(void *buf, size_t len, int flags) const = 0;
};

} // namespace rix::ipc