#ifndef TCP_CONNECTION_HPP_
# define TCP_CONNECTION_HPP_

# include <iostream>
# include <memory>
# include <utility>
# include "ITCPSocket.hh"

namespace Network
{
  template <typename T>
  class TCPConnection
  {
  private :
    using SocketPtr = std::shared_ptr<ITCPSocket>;

  private :
    SocketPtr	_socketPtr;
    T		_clientInfo;
    
  public :
    TCPConnection(const SocketPtr&);
    ~TCPConnection() = default;
    SocketPtr& getSocketPtr();
    T& getClientInfo();
  };
  
  template <typename T>
  TCPConnection<T>::TCPConnection(const SocketPtr& socketPtr) :
    _socketPtr(socketPtr)
  {
  }

  template <typename T>
  typename TCPConnection<T>::SocketPtr& TCPConnection<T>::getSocketPtr()
  {
    return _socketPtr;
  }

  template <typename T>
  T& TCPConnection<T>::getClientInfo() { return _clientInfo; }
}

#endif /* !TCP_CONNECTION_HPP_ */
