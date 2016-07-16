#ifndef HANDLER_ASYNC_WRITE_HPP_
# define HANDLER_ASYNC_WRITE_HPP_

# include <memory>
# include <boost/system/error_code.hpp>
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
  struct HandlerAsyncWrite
  {
    static void handleWrite(const std::shared_ptr
			    <Network::TCPBoostSocket<N, N2>>&,
			    const boost::system::error_code&);
  };

  template <std::size_t N, std::size_t N2>
  void HandlerAsyncWrite<N, N2>
  ::handleWrite(const std::shared_ptr
		<Network::TCPBoostSocket<N, N2>>& socketPtr,
		const boost::system::error_code& error)
  {
    if (error)
      {
	boost::any_cast
	  <typename UniSoulSystemWrapper::ConnectionManager&>
	  (socketPtr->getSystemWrapperPtrRef()
	   ->getContent()["ConnectionManager"])
	  .deleteConnectionIf
	  ([&socketPtr](const std::shared_ptr
			<Network::TCPConnection
			<Info::ClientInfo>>& connectionPtr) -> bool
	   {
	     return connectionPtr->getSocketPtr() == socketPtr;
	   });
      }
  }
}

#endif /* !HANDLER_ASYNC_WRITE_HPP_ */
