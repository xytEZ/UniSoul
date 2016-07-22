#ifndef ASYNC_READ_HANDLER_HPP_
# define ASYNC_READ_HANDLER_HPP_

# include <cstddef>
# include <memory>
# include <boost/system/error_code.hpp>
# include "UniSoulNetworkProtocolSerializable.hh"
# include "SerializationHandler.hpp"
# include "RequestExecuteFromAsyncTaskHandler.hpp"
# include "DisconnectFromAsyncTaskHandler.hpp"
# include "DeserializeErrorFromAsyncTaskHandler.hpp"

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
    SerializationHandler<UniSoulPacket>			_serializationHandler;
    
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
      DisconnectFromAsyncTaskHandler<N, N2>(_socketPtr).disconnect();
  }
  
  template <std::size_t N, std::size_t N2>
  void AsyncReadHandler<N, N2>::firstTimeReadHandle() const
  {
    std::shared_ptr
      <Serializable
       ::UniSoulNetworkProtocolSerializable>	serializablePtr
      = std::dynamic_pointer_cast
      <Serializable::UniSoulNetworkProtocolSerializable>
      (_serializationHandler.deserialize(_socketPtr->getBuffer()));
    
    if (!serializablePtr)
      DeserializeErrorFromAsyncTaskHandler<N, N2>
	(_socketPtr, _serializationHandler).deserializeError(true);
    else
      RequestExecuteFromAsyncTaskHandler<N, N2>(_socketPtr, serializablePtr)
	.requestExecute();
  }
  
  template <std::size_t N, std::size_t N2>
  void AsyncReadHandler<N, N2>::regularReadHandle() const
  {
    std::shared_ptr
      <Serializable::UniSoulNetworkProtocolSerializable>
      serializablePtr = std::dynamic_pointer_cast
      <Serializable::UniSoulNetworkProtocolSerializable>
      (_serializationHandler.deserialize(_socketPtr->getBuffer()));

    if (!serializablePtr)
      DeserializeErrorFromAsyncTaskHandler<N, N2>
	(_socketPtr, _serializationHandler).deserializeError(false);
    else
      RequestExecuteFromAsyncTaskHandler<N, N2>(_socketPtr, serializablePtr)
	.requestExecute();
  }
}

#endif /* !ASYNC_READ_HANDLER_HPP_ */
