#ifndef TCP_CONNECTION_HPP_
# define TCP_CONNECTION_HPP_

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
    ~TCPConnection();
    bool send(const std::string&);
    std::string recv();
    const SocketPtr& getSocketPtr() const;
    T& getClientInfo();
  };
  
  template <typename T>
  TCPConnection<T>::TCPConnection(const SocketPtr& socketPtr) :
    _socketPtr(socketPtr)
  {
    _socketPtr->open(0, 0, 0);
  }
  
  template <typename T>
  TCPConnection<T>::~TCPConnection() { _socketPtr->close(); }
  
  template <typename T>
  bool TCPConnection<T>::send(const std::string& msg)
  {
    return _socketPtr->send(msg);
  }

  template <typename T>
  std::string TCPConnection<T>::recv() { return _socketPtr->recv(); }

  template <typename T>
  const typename TCPConnection<T>::SocketPtr& TCPConnection<T>
  ::getSocketPtr() const
  {
    return _socketPtr;
  }

  template <typename T>
  T& TCPConnection<T>::getClientInfo() { return _clientInfo; }
}

#endif /* !TCP_CONNECTION_HPP_ */
