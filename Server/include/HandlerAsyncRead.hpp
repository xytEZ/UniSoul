#ifndef HANDLER_ASYNC_READ_HPP_
# define HANDLER_ASYNC_READ_HPP_

# include <cstddef>
# include <memory>
# include <boost/system/error_code.hpp>
# include "UniSoulSystemWrapper.hh"

namespace Network
{
  template <std::size_t N, std::size_t N2>
  class TCPBoostSocket;
}

namespace Handler
{
  template <std::size_t N, std::size_t N2>
  struct HandlerAsyncRead
  {
    static void handleRead(std::shared_ptr<Network::TCPBoostSocket<N, N2>>&,
			   const boost::system::error_code&);
  };
  
  template <std::size_t N, std::size_t N2>
  void HandlerAsyncRead<N, N2>
  ::handleRead(std::shared_ptr<Network::TCPBoostSocket<N, N2>>& socket,
	       const boost::system::error_code& error)
  {
    if (error)
      {
	boost::any_cast
	  <typename UniSoulSystemWrapper::SocketManager>
	  (socket->getSystemWrapperPtrRef()->getContent()["SocketManager"])
	  .deleteSocket(socket);
	socket->close();
      }
  }
}

#endif /* !HANDLER_ASYNC_READ_HPP_ */
