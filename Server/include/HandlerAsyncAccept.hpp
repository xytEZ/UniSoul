#ifndef HANDLER_ASYNC_ACCEPT_HPP_
# define HANDLER_ASYNC_ACCEPT_HPP_

# include <memory>
# include <boost/system/error_code.hpp>
# include "UniSoulSystemWrapper.hh"

namespace Network
{
  template <int N, int N2, typename T>
  class TCPBoostSocketServer;

  template <int N, int N2>
  class TCPBoostSocket;
}

namespace Handler
{
  template <int N, int N2, typename T>
  struct HandlerAsyncAccept
  {
    static void handleAccept(std::shared_ptr
			     <Network::TCPBoostSocketServer<N, N2, T>>&,
			     std::shared_ptr<Network::TCPBoostSocket<N, N2>>&,
			     const boost::system::error_code&);
  };
  
  template <int N, int N2, typename T>
  void HandlerAsyncAccept<N, N2, T>
  ::handleAccept(std::shared_ptr
		 <Network::TCPBoostSocketServer<N, N2, T>>& socketServer,
		 std::shared_ptr<Network::TCPBoostSocket<N, N2>>& socket,
		 const boost::system::error_code& error)
  {
    if (!error)
      {
	socket->send("Welcome to the server\n");
	socket->recv();
	boost::any_cast
	  <typename UniSoulSystemWrapper::SocketManager>
	  (socketServer->getSystemWrapperPtrRef()
	   ->getContent()["SocketManager"])
	  .addSocket(socket);
	socketServer->accept(nullptr, nullptr);
      }
  }
}

#endif /* !HANDLER_ASYNC_ACCEPT_HPP_ */
