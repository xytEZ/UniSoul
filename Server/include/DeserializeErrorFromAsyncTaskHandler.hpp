#ifndef DESERIALIZE_ERROR_FROM_ASYNC_TASK_HANDLER_HPP_
# define DESERIALIZE_ERROR_FROM_ASYNC_TASK_HANDLER_HPP_

# include <cstddef>
# include <memory>
# include "UniSoulSystemWrapper.hh"
# include "UniSoulPacketFactory.hh"
# include "ServerMessage.hh"

namespace Network
{
  template <std::size_t N, std::size_t N2>
  class TCPBoostSocket;
};

namespace Handler
{
  template <std::size_t N, std::size_t N2>
  class DeserializeErrorFromAsyncTaskHandler
  {
  private :
    const std::shared_ptr
    <Network::TCPBoostSocket<N, N2>>&		_socketPtr;
    const SerializationHandler<UniSoulPacket>&	_serializationHandler;
    Factory::UniSoulPacketFactory		_packetFactory;

  public :
    DeserializeErrorFromAsyncTaskHandler(const std::shared_ptr
					 <Network::TCPBoostSocket<N, N2>>&,
					 const SerializationHandler
					 <UniSoulPacket>&);
    ~DeserializeErrorFromAsyncTaskHandler() = default;
    void deserializeError(bool) const;
  };

  template <std::size_t N, std::size_t N2>
  DeserializeErrorFromAsyncTaskHandler<N, N2>
  ::DeserializeErrorFromAsyncTaskHandler(const std::shared_ptr
					 <Network::TCPBoostSocket<N, N2>>&
					 socketPtr,
					 const SerializationHandler
					 <UniSoulPacket>&
					 serializationHandler) :
    _socketPtr(socketPtr),
    _serializationHandler(serializationHandler)
  {
  }

  template <std::size_t N, std::size_t N2>
  void DeserializeErrorFromAsyncTaskHandler<N, N2>
  ::deserializeError(bool isFirstTime) const
  {
    if (isFirstTime)
      {
	_socketPtr->setMaintainInstance(false);
	_socketPtr->send
	  (_serializationHandler
	   .serialize(std::make_shared
		      <Serializable::UniSoulNetworkProtocolSerializable>
		      (_packetFactory.create
		       (Network::Protocol::Communication::TCP,
			Network::Protocol::Request::NONE,
			Network::Message::Server::ERROR))));
      }
    else
      _socketPtr->send
	(_serializationHandler
	 .serialize(std::make_shared
		    <Serializable::UniSoulNetworkProtocolSerializable>
		    (_packetFactory.create
		     (Network::Protocol::Communication::TCP,
		      Network::Protocol::Request::NONE,
		      Network::Message::Server::ERROR))));
    
  }
}

#endif /* !DESERIALIZE_ERROR_FROM_ASYNC_TASK_HANDLER_HPP_ */
