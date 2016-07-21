#ifndef ASYNC_READ_HANDLER_HPP_
# define ASYNC_READ_HANDLER_HPP_

# include <cstddef>
# include <memory>
# include <boost/system/error_code.hpp>
# include "DisconnectFromAsyncTaskHandler.hpp"
# include "UniSoulNetworkProtocolSerializable.hh"
# include "SerializationHandler.hpp"
# include "UniSoulPacketFactory.hh"
# include "UniSoulNetworkProtocol.hh"

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
    <Network::TCPBoostSocket<N, N2>>&	_socketPtr;
    const boost::system::error_code&	_error;
    SerializationHandler<UniSoulPacket>	_serializationHandler;
    Factory::UniSoulPacketFactory	_packetFactory;
    
  public :
    AsyncReadHandler(const std::shared_ptr<Network::TCPBoostSocket<N, N2>>&,
		     const boost::system::error_code&);
    ~AsyncReadHandler() = default;
    void readHandle() const;

  private :
    void firstTimeReadHandle() const;
    void regularReadHandle() const;
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
    static bool isFirstTime = true;

    if (!_error)
      {
	if (isFirstTime)
	  {
	    firstTimeReadHandle();
	    isFirstTime = false;
	  }
	else
	  regularReadHandle();
      }
    else
      Handler::DisconnectFromAsyncTaskHandler<N, N2>(_socketPtr).disconnect();
  }
  
  template <std::size_t N, std::size_t N2>
  void AsyncReadHandler<N, N2>::firstTimeReadHandle() const
  {
    std::shared_ptr
      <Serializable
       ::UniSoulNetworkProtocolSerializable>	serializablePtr
      = _serializationHandler.deserialize
      <Serializable::UniSoulNetworkProtocolSerializable>
      (_socketPtr->getBuffer());
    
    if (!serializablePtr)
      {
	_socketPtr->send
	  (_serializationHandler
	   .serialize(std::make_shared
		      <Serializable::UniSoulNetworkProtocolSerializable>
		      (_packetFactory.create
		       (Network::e_communication::TCP,
			Network::e_request::ERROR,
			"An error is occurred during your identification. Try again."))));
      }
  }
  
  template <std::size_t N, std::size_t N2>
  void AsyncReadHandler<N, N2>::regularReadHandle() const
  {
  }
}

#endif /* !ASYNC_READ_HANDLER_HPP_ */
