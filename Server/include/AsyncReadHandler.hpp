#ifndef ASYNC_READ_HANDLER_HPP_
# define ASYNC_READ_HANDLER_HPP_

# include <cstddef>
# include <memory>
# include <boost/system/error_code.hpp>

# include "DisconnectFromAsyncTaskHandler.hpp"
# include "RequestProcessingFromAsyncTaskHandler.hpp"
# include "UniSoulPacketFactory.hh"
# include "UniSoulNetworkProtocolSerializable.hh"
# include "BoostDescriptor.hh"

namespace Network
{
  template <std::size_t N, std::size_t N2>
  class TCPBoostSocket;
}

namespace Handler
{
  template <std::size_t N, std::size_t N2>
  class AsyncReadHandler
  {
  private :
    std::shared_ptr<Network::TCPBoostSocket<N, N2>>	_socketPtr;
    boost::system::error_code				_error;
    
  public :
    AsyncReadHandler(const std::shared_ptr<Network::TCPBoostSocket<N, N2>>&,
		     const boost::system::error_code&);
    
    ~AsyncReadHandler() = default;
    void readHandle() const;
  };

  template <std::size_t N, std::size_t N2>
  AsyncReadHandler<N, N2>
  ::AsyncReadHandler(const std::shared_ptr
		     <Network::TCPBoostSocket<N, N2>>& socketPtr,
		     const boost::system::error_code& error) :
    _socketPtr(socketPtr),
    _error(error)
  {
  }
  
  template <std::size_t N, std::size_t N2>
  void AsyncReadHandler<N, N2>::readHandle() const
  {
    if (_error)
	RequestProcessingFromAsyncTaskHandler
	  <Network::Protocol::UniSoulPacket,
	   Factory::UniSoulPacketFactory,
	   Serializable::UniSoulNetworkProtocolSerializable,
	   N,
	   N2>(_socketPtr).requestProcessing();
    else
      DisconnectFromAsyncTaskHandler<N, N2>(_socketPtr)
	.disconnect
	(boost::any_cast
	 <typename Wrapper::UniSoulSystemWrapper::ConnectionManager&>
	 (_socketPtr->getSystemWrapperPtrRef()
	  ->getContent()["ConnectionManager"])
	 .findConnectionIf([this](const std::shared_ptr
				  <Network::TCPConnection
				  <Info::ClientInfo, ::Descriptor>>&
				  connectionPtr) -> bool
			   {
			     return connectionPtr
			       ->getSocketPtr() == _socketPtr;
			   }));
  }
}

#endif /* !ASYNC_READ_HANDLER_HPP_ */
