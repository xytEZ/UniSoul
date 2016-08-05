#ifndef ITCP_SOCKET_SERVER_HPP_
# define ITCP_SOCKET_SERVER_HPP_

namespace Network
{
  template <typename T>
  class ITCPSocket;
  
  template <typename T, typename U = int>
  class ITCPSocketServer : virtual public ITCPSocket<T>
  {
  public :
    virtual ~ITCPSocketServer() = default;
    virtual bool bind() = 0;
    virtual bool listen() = 0;
    virtual U accept() = 0;
  };
}

#endif /* ! ITCP_SOCKET_SERVER_HPP_ */
