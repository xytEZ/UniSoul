#ifndef ITCP_SOCKET_SERVER_HPP_
# define ITCP_SOCKET_SERVER_HPP_

# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/ip.h>

namespace Network
{
  template <typename T>
  class ITCPSocket;
  
  using t_sockaddr = struct sockaddr;
  
  template <typename T, typename U = int>
  class ITCPSocketServer : virtual public ITCPSocket<T>
  {
  public :
    virtual ~ITCPSocketServer() = default;
    virtual bool bind(const t_sockaddr *, int) = 0;
    virtual bool listen(int) = 0;
    virtual U accept(t_sockaddr *, int *) = 0;
  };
}

#endif /* ! ITCP_SOCKET_SERVER_HPP_ */
