#ifndef DISCONNECT_FROM_ASYNC_TASK_HANDLER_HPP_
# define DISCONNECT_FROM_ASYNC_TASK_HANDLER_HPP_

# include <cstddef>
# include <memory>

# include "UniSoulSystemWrapper.hh"
# include "ClientInfo.hh"
# include "TCPConnection.hpp"

namespace Network
{
  template <typename T>
  class ITCPSocket;
}

namespace Handler
{
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
    void disconnect(bool) const;
  };

  template <std::size_t N, std::size_t N2>
  DisconnectFromAsyncTaskHandler<N, N2>
  ::DisconnectFromAsyncTaskHandler(const std::shared_ptr<Network::ITCPSocket
				   <boost::asio::ip::tcp::socket>>& socketPtr) :
    _socketPtr(socketPtr)
  {
  }
      
  template <std::size_t N, std::size_t N2>
  void DisconnectFromAsyncTaskHandler<N, N2>
  ::disconnect(bool registeredConnection) const
  {
    if (registeredConnection)
      boost::any_cast
	<typename Wrapper::UniSoulSystemWrapper::ConnectionManager&>
	(std::static_pointer_cast<Network::TCPBoostSocketServer<N, N2>>
	 (_socketPtr)->getSystemWrapperPtrRef()
	 ->getContent()["ConnectionManager"])
	.deleteConnectionIf
	([this](const std::shared_ptr
		<Network::TCPConnection
		<Info::ClientInfo,
		boost::asio::ip::tcp::socket>>& connectionPtr) -> bool
	 {
	 return connectionPtr->getSocketPtr() == _socketPtr;
       });
    _socketPtr->close();
  }
}

#endif /* !DISCONNECT_FROM_ASYNC_TASK_HANDLER_HPP_ */
