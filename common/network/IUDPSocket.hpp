#ifndef IUDP_SOCKET_HPP_
# define IUDP_SOCKET_HPP_

# include <string>

# include "ISocket.hpp"

struct sockaddr;

namespace Network
{
  template <typename T>
  class IUDPSocket : public ISocket<T>
  {
  public :
    virtual ~IUDPSocket() = default;
    virtual bool sendTo(const std::string&, const struct sockaddr *) = 0;
    virtual std::string recvFrom(struct sockaddr *) = 0;
  };
}

#endif /* !IUDP_SOCKET_HPP_ */
