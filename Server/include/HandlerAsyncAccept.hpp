#ifndef HANDLER_ASYNC_ACCEPT_HPP_
# define HANDLER_ASYNC_ACCEPT_HPP_

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
  struct HandlerAsyncAccept
  {
    static void handleAccept(std::shared_ptr
			     <Network::TCPBoostSocketServer<N, N2, T>>&,
			     std::shared_ptr<Network::TCPBoostSocket<N, N2>>&,
			     const boost::system::error_code&);
  };
  
  template <std::size_t N, std::size_t N2, typename T>
  void HandlerAsyncAccept<N, N2, T>
  ::handleAccept(std::shared_ptr
		 <Network::TCPBoostSocketServer<N, N2, T>>& serverSocketPtr,
		 std::shared_ptr<Network::TCPBoostSocket<N, N2>>& socketPtr,
		 const boost::system::error_code& error)
  {
    if (!error)
      {
	std::shared_ptr
	  <Network::TCPConnection<Info::ClientInfo>> connectionPtr =
	  std::make_shared<Network::TCPConnection
			   <Info::ClientInfo>>(socketPtr);

	connectionPtr->send("Welcome to the server\n");
	connectionPtr->recv();
	boost::any_cast
	  <typename UniSoulSystemWrapper::ConnectionManager&>
	  (serverSocketPtr->getSystemWrapperPtrRef()
	   ->getContent()["ConnectionManager"])
	  .addConnection(connectionPtr);
	std::cout << boost::any_cast
	  <typename UniSoulSystemWrapper::ConnectionManager&>
	  (serverSocketPtr->getSystemWrapperPtrRef()
	   ->getContent()["ConnectionManager"])
	   .size() << std::endl;
	serverSocketPtr->accept(nullptr, nullptr);
      }
  }
}

#endif /* !HANDLER_ASYNC_ACCEPT_HPP_ */
