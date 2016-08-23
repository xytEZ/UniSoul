#ifndef ITCP_SOCKET_HPP_
# define ITCP_SOCKET_HPP_

# include <string>

# include "RemoteConnectionInfo.hh"
# include "ISocket.hpp"

namespace Network
{
  template <typename T>
  class ITCPSocket : public ISocket<T>
  {
  public :
    virtual ~ITCPSocket() = default;
    virtual bool send(const std::string&) = 0;
    virtual std::string recv() = 0;
    virtual std::string getAddress() const = 0;
    virtual unsigned short getPort() const = 0;
    virtual const RemoteConnectionInfo& getRemoteConnectionInfo() const = 0;
    virtual void setRemoteConnectionInfo(const RemoteConnectionInfo&) = 0;
  };
}

#endif /* !ITCP_SOCKET_HPP_ */
