#include "rix/ipc/lwip_socket.hpp"

namespace rix::ipc {

LWIPSocket::LWIPSocket() : s_(::socket(AF_INET, SOCK_STREAM, 0)) {}

LWIPSocket::LWIPSocket(int fd) : s_(fd) {}

LWIPSocket::~LWIPSocket() { close(); }

bool LWIPSocket::bind(const Endpoint &endpoint) const {
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(endpoint.port);
  inet_pton(AF_INET, endpoint.address.c_str(), &addr.sin_addr);
  return ::bind(s_, (struct sockaddr *)&addr, sizeof(addr)) == 0;
}

bool LWIPSocket::listen(int backlog) const {
  return ::listen(s_, backlog) == 0;
}

std::unique_ptr<GenericSocket>
LWIPSocket::accept(Endpoint &remote_endpoint) const {
  struct sockaddr_in addr;
  socklen_t len = sizeof(addr);
  int sock_fd = ::accept(s_, (struct sockaddr *)&addr, &len);
  if (sock_fd < 0) {
    return nullptr;
  }
  remote_endpoint.address.resize(INET_ADDRSTRLEN);
  inet_ntop(AF_INET, &addr.sin_addr, remote_endpoint.address.data(),
            INET_ADDRSTRLEN);
  remote_endpoint.port = ntohs(addr.sin_port);
  return std::unique_ptr<LWIPSocket>(new LWIPSocket(sock_fd));
}

bool LWIPSocket::connect(const Endpoint &endpoint) const {
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(endpoint.port);
  inet_pton(AF_INET, endpoint.address.c_str(), &addr.sin_addr);
  return ::connect(s_, (struct sockaddr *)&addr, sizeof(addr)) == 0;
}

void LWIPSocket::close() const { ::close(s_); }

ssize_t LWIPSocket::send(const void *buf, size_t len, int flags) const {
  return ::send(s_, buf, len, flags);
}

ssize_t LWIPSocket::recv(void *buf, size_t len, int flags) const {
  return ::recv(s_, buf, len, flags);
}

ssize_t LWIPSocket::send_to(const void *buf, size_t len,
                            const Endpoint &endpoint, int flags) const {
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(endpoint.port);
  inet_pton(AF_INET, endpoint.address.c_str(), &addr.sin_addr);
  return ::sendto(s_, buf, len, flags, (struct sockaddr *)&addr, sizeof(addr));
}

ssize_t LWIPSocket::recv_from(void *buf, size_t len, Endpoint &endpoint,
                              int flags) const {
  struct sockaddr_in addr;
  socklen_t addrlen = sizeof(addr);
  ssize_t n;
  n = ::recvfrom(s_, buf, len, flags, (struct sockaddr *)&addr, &addrlen);
  if (n < 0) {
    return n;
  }
  endpoint.address.resize(INET_ADDRSTRLEN);
  inet_ntop(AF_INET, &addr.sin_addr, endpoint.address.data(), INET_ADDRSTRLEN);
  endpoint.port = ntohs(addr.sin_port);
  return n;
}

bool LWIPSocket::wait_readable(const rix::util::Duration &timeout) const {
  fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(s_, &readfds);
  struct timeval tv;
  tv.tv_sec = timeout.to_nanoseconds() / 1'000'000'000;
  tv.tv_usec = (timeout.to_nanoseconds() % 1'000'000'000) / 1'000;
  int ret = select(s_ + 1, &readfds, NULL, NULL, &tv);
  return ret > 0 && FD_ISSET(s_, &readfds);
}

bool LWIPSocket::wait_writable(const rix::util::Duration &timeout) const {
  fd_set writefds;
  FD_ZERO(&writefds);
  FD_SET(s_, &writefds);
  struct timeval tv;
  tv.tv_sec = timeout.to_nanoseconds() / 1'000'000'000;
  tv.tv_usec = (timeout.to_nanoseconds() % 1'000'000'000) / 1'000;
  int ret = select(s_ + 1, NULL, &writefds, NULL, &tv);
  return ret > 0 && FD_ISSET(s_, &writefds);
}

bool LWIPSocket::wait_exception(const rix::util::Duration &timeout) const {
  fd_set exceptfds;
  FD_ZERO(&exceptfds);
  FD_SET(s_, &exceptfds);
  struct timeval tv;
  tv.tv_sec = timeout.to_nanoseconds() / 1'000'000'000;
  tv.tv_usec = (timeout.to_nanoseconds() % 1'000'000'000) / 1'000;
  int ret = select(s_ + 1, NULL, NULL, &exceptfds, &tv);
  return ret > 0 && FD_ISSET(s_, &exceptfds);
}

bool LWIPSocket::set_blocking(bool blocking) const {
  int flags = fcntl(s_, F_GETFL, 0);
  if (flags < 0) {
    return false;
  }
  if (blocking) {
    flags &= ~O_NONBLOCK;
  } else {
    flags |= O_NONBLOCK;
  }
  return fcntl(s_, F_SETFL, flags) == 0;
}

bool LWIPSocket::get_blocking() const {
  int flags = fcntl(s_, F_GETFL, 0);
  if (flags < 0) {
    return false;
  }
  return (flags & O_NONBLOCK) == 0;
}

bool LWIPSocket::set_reuse_address(bool reuse) const {
  int optval = reuse ? 1 : 0;
  int status;
  status = setsockopt(s_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  return status == 0;
}

bool LWIPSocket::get_reuse_address() const {
  int optval;
  socklen_t optlen = sizeof(optval);
  if (getsockopt(s_, SOL_SOCKET, SO_REUSEADDR, &optval, &optlen) < 0) {
    return false;
  }
  return optval != 0;
}

bool LWIPSocket::set_reuse_port(bool reuse) const {
  int optval = reuse ? 1 : 0;
  int status;
  status = setsockopt(s_, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
  return status == 0;
}

bool LWIPSocket::get_reuse_port() const {
  int optval;
  socklen_t optlen = sizeof(optval);
  if (getsockopt(s_, SOL_SOCKET, SO_REUSEPORT, &optval, &optlen) < 0) {
    return false;
  }
  return optval != 0;
}

bool LWIPSocket::set_recv_buffer_size(int size) const {
  return setsockopt(s_, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size)) == 0;
}

int LWIPSocket::get_recv_buffer_size() const {
  int size;
  socklen_t optlen = sizeof(size);
  if (getsockopt(s_, SOL_SOCKET, SO_RCVBUF, &size, &optlen) < 0) {
    return -1;
  }
  return size;
}

bool LWIPSocket::set_send_buffer_size(int size) const {
  return setsockopt(s_, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size)) == 0;
}

int LWIPSocket::get_send_buffer_size() const {
  int size;
  socklen_t optlen = sizeof(size);
  if (getsockopt(s_, SOL_SOCKET, SO_SNDBUF, &size, &optlen) < 0) {
    return -1;
  }
  return size;
}

bool LWIPSocket::join_multicast_group(
    const std::string &multicast_address) const {
  ip_mreq mreq;
  mreq.imr_multiaddr.s_addr = inet_addr(multicast_address.c_str());
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);
  int status;
  status = setsockopt(s_, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
  return status == 0;
}

bool LWIPSocket::leave_multicast_group(
    const std::string &multicast_address) const {
  ip_mreq mreq;
  mreq.imr_multiaddr.s_addr = inet_addr(multicast_address.c_str());
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);
  int status;
  status = setsockopt(s_, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));
  return status == 0;
}

Endpoint LWIPSocket::local_endpoint() const {
  struct sockaddr_in addr;
  socklen_t addrlen = sizeof(addr);
  if (getsockname(s_, (struct sockaddr *)&addr, &addrlen) < 0) {
    return Endpoint();
  }
  Endpoint ep;
  ep.address.resize(INET_ADDRSTRLEN);
  inet_ntop(AF_INET, &addr.sin_addr, ep.address.data(), INET_ADDRSTRLEN);
  ep.port = ntohs(addr.sin_port);
  return ep;
}

Endpoint LWIPSocket::remote_endpoint() const {
  struct sockaddr_in addr;
  socklen_t addrlen = sizeof(addr);
  if (getpeername(s_, (struct sockaddr *)&addr, &addrlen) < 0) {
    return Endpoint();
  }
  Endpoint ep;
  ep.address.resize(INET_ADDRSTRLEN);
  inet_ntop(AF_INET, &addr.sin_addr, ep.address.data(), INET_ADDRSTRLEN);
  ep.port = ntohs(addr.sin_port);
  return ep;
}

} // namespace rix::ipc