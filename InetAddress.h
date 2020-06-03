#ifndef MUDUO_NET_INETADDRESS_H
#define MUDUO_NET_INETADDRESS_H

#include "copyable.h"

#include <string>
#include <netinet/in.h>

namespace muduo
{

/// Wrapper of sockaddr_in. This is an POD interface class.
class InetAddress : public muduo::copyable
{
 public:
  explicit InetAddress(uint16_t port);

  /// Constructs an endpoint with given ip and port. ip should be "1.2.3.4"
  InetAddress(const std::string& ip, uint16_t port);

  InetAddress(const struct sockaddr_in& addr)
    : addr_(addr)
  { }

  std::string toHostPort() const;
  const struct sockaddr_in& getSockAddrInet() const { return addr_; }
  void setSockAddrInet(const struct sockaddr_in& addr) { addr_ = addr; }

 private:
  struct sockaddr_in addr_;
};

}

#endif  // MUDUO_NET_INETADDRESS_H
