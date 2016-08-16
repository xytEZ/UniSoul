#ifndef ASYNC_READ_HANDLER_HPP_
# define ASYNC_READ_HANDLER_HPP_

# include <cstddef>
# include <memory>
# include <boost/system/error_code.hpp>

# include "DisconnectFromAsyncTaskHandler.hpp"
# include "RequestProcessingFromAsyncTaskHandler.hpp"
# include "UniSoulPacketFactory.hh"

namespace Network
{
  template <typename T>
  class ITCPSocket;
  
  template <std::size_t N, std::size_t N2>
  class AsyncReadHandler
  {
  private :
    std::shared_ptr<Network::ITCPSocket
    <boost::asio::ip::tcp::socket>>			_socketPtr;
    
    boost::system::error_code				_error;
    
  public :
    AsyncReadHandler(const std::shared_ptr<Network::ITCPSocket
		     <boost::asio::ip::tcp::socket>>&,
		     const boost::system::error_code&);
    
    ~AsyncReadHandler() = default;
    void readHandle() const;
  };

  template <std::size_t N, std::size_t N2>
  AsyncReadHandler<N, N2>
  ::AsyncReadHandler(const std::shared_ptr<Network::ITCPSocket
		     <boost::asio::ip::tcp::socket>>& socketPtr,
		     const boost::system::error_code& error) :
    _socketPtr(socketPtr),
    _error(error)
  {
  }
  
  template <std::size_t N, std::size_t N2>
  void AsyncReadHandler<N, N2>::readHandle() const
  {
    if (!_error)
      {
	RequestProcessingFromAsyncTaskHandler
	  <Network::Protocol::UniSoulPacket,
	   Factory::UniSoulPacketFactory,
	   N,
	   N2>
	  (_socketPtr).requestProcessing();
      }
    else
      DisconnectFromAsyncTaskHandler<N, N2>(_socketPtr)
	.disconnect
	(boost::any_cast
	 <typename Wrapper::UniSoulSystemWrapper::SocketManager&>
	 (std::static_pointer_cast<Network::TCPBoostSocketServer<N, N2>>
	  (_socketPtr)->getSystemWrapperPtrRef()
	  ->getContent()["SocketManager"])
	 .findSocketPtrIf([this]
			  (const std::pair<std::string, std::shared_ptr
			   <Network::ISocket<boost::asio::ip::tcp::socket>>>&
			   pairSocketPtr) -> bool
			   {
			     return pairSocketPtr.second == _socketPtr;
			   }));
  }
}

#endif /* !ASYNC_READ_HANDLER_HPP_ */
