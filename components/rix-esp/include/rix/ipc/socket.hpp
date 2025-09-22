#pragma once

#include "rix/ipc/generic_socket.hpp"
#include "rix/ipc/lwip_socket.hpp"
#include <memory>

namespace rix::ipc {
using Socket = rix::ipc::LWIPSocket;

static inline std::shared_ptr<GenericSocket> create_socket() {
  return std::make_shared<Socket>();
}

enum class SelectFlag { READ = 1, WRITE = 2 };

template <typename Iterator, typename T = typename std::iterator_traits<Iterator>::value_type>
bool select(std::vector<std::shared_ptr<GenericSocket>> &sockets,
            std::vector<std::shared_ptr<GenericSocket>> &exception_sockets, Iterator begin, Iterator end,
            const rix::util::Duration &duration, SelectFlag flag) {
  static_assert(std::is_same<T, std::shared_ptr<GenericSocket>>::value,
                "T must be a shared_ptr to a type derived from GenericSocket");
  if (begin == end) {
    return true;
  }

  fd_set fds;
  fd_set exception_fds;
  FD_ZERO(&fds);
  FD_ZERO(&exception_fds);
  int max_fd = -1;
  for (auto it = begin; it != end; ++it) {
    int fd = (*it)->get_fd();
    FD_SET(fd, &fds);
    FD_SET(fd, &exception_fds);
    if (fd > max_fd) {
      max_fd = fd;
    }
  }
  if (max_fd < 0) {
    return false;
  }

  struct timeval tv;
  int64_t ns = duration.to_nanoseconds();
  tv.tv_sec = static_cast<long>(ns / 1'000'000'000);
  tv.tv_usec = static_cast<long>(ns % 1'000'000'000 / 1'000);

  int ret =
      ::select(max_fd + 1, (flag == SelectFlag::READ) ? &fds : nullptr, (flag == SelectFlag::WRITE) ? &fds : nullptr,
               &exception_fds, (duration.to_nanoseconds() < 0) ? nullptr : &tv);
  if (ret < 0) {
    return false;
  }

  sockets.clear();
  sockets.reserve(std::distance(begin, end));
  for (auto it = begin; it != end; ++it) {
    int fd = (*it)->get_fd();
    if (FD_ISSET(fd, &fds)) {
      sockets.push_back(*it);
    }
    if (FD_ISSET(fd, &exception_fds)) {
      exception_sockets.push_back(*it);
    }
  }
  sockets.shrink_to_fit();
  return true;
}
} // namespace rix::ipc