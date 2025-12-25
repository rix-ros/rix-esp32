#pragma once

#include "rix/ipc/generic_socket.hpp"
#include "rix/util/time.hpp"

#include "lwip/sockets.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include <memory>

namespace rix::ipc {

class LWIPSocket : public GenericSocket {
public:
  LWIPSocket();
  ~LWIPSocket() override;

  virtual bool bind(const Endpoint &endpoint) const override;
  virtual bool listen(int backlog) const override;
  virtual std::shared_ptr<GenericSocket>
  accept(Endpoint &remote_endpoint) const override;
  virtual bool connect(const Endpoint &endpoint) const override;
  virtual void close() const override;
  ssize_t writev(const ConstMessageSegment *segments,
                 size_t segment_count) const override;
  ssize_t readv(MessageSegment *segments, size_t segment_count) const override;
  virtual ssize_t send(const void *buf, size_t len, int flags) const override;
  virtual ssize_t recv(void *buf, size_t len, int flags) const override;
  virtual bool wait_readable(const rix::Duration &timeout) const override;
  virtual bool wait_writable(const rix::Duration &timeout) const override;
  virtual bool wait_exception(const rix::Duration &timeout) const override;
  virtual bool set_blocking(bool blocking) const override;
  virtual bool get_blocking() const override;
  virtual bool set_reuse_address(bool reuse) const override;
  virtual bool get_reuse_address() const override;
  virtual Endpoint local_endpoint() const override;
  virtual Endpoint remote_endpoint() const override;
  virtual int get_fd() const override;

private:
  LWIPSocket(int s);
  int s_;
  mutable bool is_blocking_;
};

} // namespace rix::ipc