#ifndef ASYNC_ACCEPT_HANDLER_HPP_
# define ASYNC_ACCEPT_HANDLER_HPP_

# include <cstddef>
# include <memory>
# include <boost/system/error_code.hpp>
# include "UniSoulSystemWrapper.hh"
# include "ClientInfo.hh"
# include "TCPConnection.hpp"
# include "ConnectionManager.hpp"

namespace Network
{
  template <std::size_t N, std::size_t N2, typename T>
  class TCPBoostSocketServer;

  template <std::size_t N, std::size_t N2>
  class TCPBoostSocket;
}

namespace Handler
{
  template <std::size_t N, std::size_t N2, typename T>
  class AsyncAcceptHandler
  {
  private :
    const std::shared_ptr
    <Network::TCPBoostSocketServer<N, N2, T>>&	_serverSocketPtr;
    const std::shared_ptr
    <Network::TCPBoostSocket<N, N2>>&		_socketPtr;
    const boost::system::error_code&		_error;
    
  public :
    AsyncAcceptHandler(const std::shared_ptr
		       <Network::TCPBoostSocketServer<N, N2, T>>&,
		       const std::shared_ptr
		       <Network::TCPBoostSocket<N, N2>>&,
		       const boost::system::error_code&);
    ~AsyncAcceptHandler() = default;
    void acceptHandle() const;
  };

  template <std::size_t N, std::size_t N2, typename T>
  AsyncAcceptHandler<N, N2, T>
  ::AsyncAcceptHandler(const std::shared_ptr
		       <Network::TCPBoostSocketServer<N, N2, T>>&
		       serverSocketPtr,
		       const std::shared_ptr
		       <Network::TCPBoostSocket<N, N2>>& socketPtr,
		       const boost::system::error_code& error) :
    _serverSocketPtr(serverSocketPtr),
    _socketPtr(socketPtr),
    _error(error)
  {
  }
  
  template <std::size_t N, std::size_t N2, typename T>
  void AsyncAcceptHandler<N, N2, T>::acceptHandle() const
  {
    if (!_error)
      {
	std::shared_ptr
	  <Network::TCPConnection<Info::ClientInfo>> connectionPtr =
	  std::make_shared<Network::TCPConnection
			   <Info::ClientInfo>>(_socketPtr);

	connectionPtr->send("Welcome to the server\n");
	connectionPtr->recv();
	boost::any_cast
	  <typename UniSoulSystemWrapper::ConnectionManager&>
	  (_serverSocketPtr->getSystemWrapperPtrRef()
	   ->getContent()["ConnectionManager"])
	  .addConnection(connectionPtr);
	_serverSocketPtr->accept(nullptr, nullptr);
      }
  }
}

#endif /* !ASYNC_ACCEPT_HANDLER_HPP_ */
