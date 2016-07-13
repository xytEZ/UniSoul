#ifndef HANDLER_ASYNC_WRITE_HPP_
# define HANDLER_ASYNC_WRITE_HPP_

# include <memory>
# include <boost/system/error_code.hpp>
# include "UniSoulSystemWrapper.hh"

namespace Network
{
  template <int N, int N2>
  class TCPBoostSocket;
}

namespace Handler
{
  template <int N, int N2>
  struct HandlerAsyncWrite
  {
    static void handleWrite(std::shared_ptr<Network::TCPBoostSocket<N, N2>>&,
			    const boost::system::error_code&);
  };

  template <int N, int N2>
  void HandlerAsyncWrite<N, N2>
  ::handleWrite(std::shared_ptr<Network::TCPBoostSocket<N, N2>>& socket,
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

#endif /* !HANDLER_ASYNC_WRITE_HPP_ */
