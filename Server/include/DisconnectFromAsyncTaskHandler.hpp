#ifndef DISCONNECT_FROM_ASYNC_TASK_HANDLER_HPP_
# define DISCONNECT_FROM_ASYNC_TASK_HANDLER_HPP_

# include <cstddef>
# include <memory>
# include "UniSoulSystemWrapper.hh"
# include "ClientInfo.hh"
# include "TCPConnection.hpp"

namespace Network
{
  template <std::size_t N, std::size_t N2>
  class TCPBoostSocket;
}

namespace Handler
{
  template <std::size_t N, std::size_t N2>
  class DisconnectFromAsyncTaskHandler
  {
  private :
    const std::shared_ptr<Network::TCPBoostSocket<N, N2>>&	_socketPtr;
    
  public :
    DisconnectFromAsyncTaskHandler(const std::shared_ptr
				   <Network::TCPBoostSocket<N, N2>>&);
    ~DisconnectFromAsyncTaskHandler() = default;
    void disconnect() const;
  };

  template <std::size_t N, std::size_t N2>
  DisconnectFromAsyncTaskHandler<N, N2>
  ::DisconnectFromAsyncTaskHandler(const std::shared_ptr
				   <Network::TCPBoostSocket<N, N2>>&
				   socketPtr) :
    _socketPtr(socketPtr)
  {
  }
    
  
  template <std::size_t N, std::size_t N2>
  void DisconnectFromAsyncTaskHandler<N, N2>::disconnect() const
  {
    boost::any_cast
      <typename UniSoulSystemWrapper::ConnectionManager&>
      (_socketPtr->getSystemWrapperPtrRef()
       ->getContent()["ConnectionManager"])
      .deleteConnectionIf
      ([this](const std::shared_ptr
	      <Network::TCPConnection
	      <Info::ClientInfo>>& connectionPtr) -> bool
       {
	 return connectionPtr->getSocketPtr() == _socketPtr;
       });
  }
}

#endif /* !DISCONNECT_FROM_ASYNC_TASK_HANDLER_HPP_ */
