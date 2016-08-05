#ifndef ITCP_SOCKET_CLIENT_HPP_
# define ITCP_SOCKET_CLIENT_HPP_

# include "ITCPSocket.hpp"

namespace Network
{
  template <typename T>
  class ITCPSocketClient : public ITCPSocket<T>
  { 
  public :
    virtual ~ITCPSocketClient() = default;
    virtual bool connect() = 0;
  };
}

#endif /* !ITCP_SOCKET_CLIENT_HPP_ */
