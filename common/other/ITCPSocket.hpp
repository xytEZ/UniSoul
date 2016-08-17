#ifndef ITCP_SOCKET_HPP_
# define ITCP_SOCKET_HPP_

# include <string>
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
    virtual const std::string& getRecipient() const = 0;
    virtual void setRecipient(const std::string&) = 0;
  };
}

#endif /* !ITCP_SOCKET_HPP_ */
