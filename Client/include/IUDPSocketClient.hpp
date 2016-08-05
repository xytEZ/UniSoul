#ifndef IUDP_SOCKET_CLIENT_HPP_
# define IUDP_SOCKET_CLIENT_HPP_

# include "IUDPSocket.hpp"

namespace Network
{
  template <typename T>
  class IUDPSocketClient : public IUDPSocket<T>
  {
  public :
    virtual ~IUDPSocketClient() = default;
  };
}

#endif /* !IUDP_SOCKET_CLIENT_HPP_ */
