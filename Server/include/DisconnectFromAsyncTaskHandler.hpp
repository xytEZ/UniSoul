#ifndef DISCONNECT_FROM_ASYNC_TASK_HANDLER_HPP_
# define DISCONNECT_FROM_ASYNC_TASK_HANDLER_HPP_

# include <cstddef>
# include <memory>
# include <utility>

# include "UniSoulSystemWrapper.hh"

namespace Network
{
  template <typename T>
  class ITCPSocket;
  
  template <std::size_t N, std::size_t N2>
  class DisconnectFromAsyncTaskHandler
  {
  private :
    const std::shared_ptr<Network::ITCPSocket
    <boost::asio::ip::tcp::socket>>&		_socketPtr;
    
  public :
    DisconnectFromAsyncTaskHandler(const std::shared_ptr<Network::ITCPSocket
				   <boost::asio::ip::tcp::socket>>&);
    ~DisconnectFromAsyncTaskHandler() = default;
    bool disconnect(bool) const;
  };

  template <std::size_t N, std::size_t N2>
  DisconnectFromAsyncTaskHandler<N, N2>
  ::DisconnectFromAsyncTaskHandler(const std::shared_ptr<Network::ITCPSocket
				   <boost::asio::ip::tcp::socket>>& socketPtr) :
    _socketPtr(socketPtr)
  {
  }
      
  template <std::size_t N, std::size_t N2>
  bool DisconnectFromAsyncTaskHandler<N, N2>
  ::disconnect(bool registeredConnection) const
  {
    if (registeredConnection)
      boost::any_cast
	<typename Wrapper::UniSoulSystemWrapper::SocketManager&>
	(std::static_pointer_cast<Network::TCPBoostSocketServer<N, N2>>
	 (_socketPtr)->getSystemWrapperPtrRef()
	 ->getContent()["SocketManager"])
	.deleteSocketPtrIf
	([this](const std::shared_ptr
		<Network::ISocket<boost::asio::ip::tcp::socket>>&
		socketPtr) -> bool
	 {
	   return socketPtr == _socketPtr;
	 });
    _socketPtr->close();
    return true;
  }
}

#endif /* !DISCONNECT_FROM_ASYNC_TASK_HANDLER_HPP_ */
