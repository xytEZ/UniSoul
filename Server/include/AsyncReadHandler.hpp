#ifndef ASYNC_READ_HANDLER_HPP_
# define ASYNC_READ_HANDLER_HPP_

# include <cstddef>
# include <memory>
# include <boost/system/error_code.hpp>
# include "DisconnectFromAsyncTaskHandler.hpp"
# include "RequestProcessingFromAsyncTaskHandler.hpp"
# include "UniSoulPacketFactory.hh"
# include "UniSoulNetworkProtocolSerializable.hh"
# include "UniSoulPacketStateChecker.hh"

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
    const std::shared_ptr
    <Network::TCPBoostSocket<N, N2>>&			_socketPtr;
    const boost::system::error_code&			_error;
    
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
    static bool	isFirstTime = true;
    
    if (_error)
      {
	RequestProcessingFromAsyncTaskHandler
	  <Network::Protocol::UniSoulPacket,
	   Factory::UniSoulPacketFactory,
	   Serializable::UniSoulNetworkProtocolSerializable,
	   Network::UniSoulPacketStateChecker,
	   N,
	   N2>(_socketPtr).requestProcessing(isFirstTime);
	if (isFirstTime)
	  isFirstTime = false;
      }
    else
      DisconnectFromAsyncTaskHandler<N, N2>(_socketPtr).disconnect();
  }
}

#endif /* !ASYNC_READ_HANDLER_HPP_ */
