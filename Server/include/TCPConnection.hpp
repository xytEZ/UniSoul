#ifndef TCP_CONNECTION_HPP_
# define TCP_CONNECTION_HPP_

# include <iostream>
# include <memory>
# include <utility>

# include "ITCPSocket.hpp"

namespace Network
{
  template <typename T, typename U = int>
  class TCPConnection
  {
  private :
    using SocketPtr = std::shared_ptr<ITCPSocket<U>>;

  private :
    SocketPtr	_socketPtr;
    T		_clientInfo;
    
  public :
    TCPConnection(const SocketPtr&);
    ~TCPConnection() = default;
    SocketPtr& getSocketPtr();
    T& getClientInfo();
  };
  
  template <typename T, typename U>
  TCPConnection<T, U>::TCPConnection(const SocketPtr& socketPtr) :
    _socketPtr(socketPtr)
  {
  }

  template <typename T, typename U>
  typename TCPConnection<T, U>::SocketPtr& TCPConnection<T, U>::getSocketPtr()
  {
    return _socketPtr;
  }

  template <typename T, typename U>
  T& TCPConnection<T, U>::getClientInfo() { return _clientInfo; }
}

#endif /* !TCP_CONNECTION_HPP_ */
