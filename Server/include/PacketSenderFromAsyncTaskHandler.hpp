#ifndef PACKET_SENDER_FROM_ASYNC_TASK_HANDLER_HPP_
# define PACKET_SENDER_FROM_ASYNC_TASK_HANDLER_HPP_

# include <cstddef>
# include <memory>
# include <string>
# include "ServerMessage.hh"
# include "SerializationException.hh"

namespace Network
{
  template <std::size_t N, std::size_t N2>
  class TCPBoostSocket;
}

namespace Serializable
{
  class UniSoulNetworkProtocolSerializable;
}

namespace Handler
{
  template <typename T, typename U, typename V, std::size_t N, std::size_t N2>
  class PacketSenderFromAsyncTaskHandler
  {
  private :
    const std::shared_ptr
    <Network::TCPBoostSocket<N, N2>>&	_socketPtr;
    const SerializationHandler<T>&	_serializationHandler;
    U					_packetFactory;

  public :
    PacketSenderFromAsyncTaskHandler(const std::shared_ptr
				     <Network::TCPBoostSocket<N, N2>>&,
				     const SerializationHandler<T>&);
    ~PacketSenderFromAsyncTaskHandler() = default;
    void packetError() const;
    void packetSuccess(const std::string&) const;
  };

  template <typename T, typename U, typename V, std::size_t N, std::size_t N2>
  PacketSenderFromAsyncTaskHandler<T, U, V, N, N2>
  ::PacketSenderFromAsyncTaskHandler(const std::shared_ptr
				     <Network::TCPBoostSocket<N, N2>>&
				     socketPtr,
				     const SerializationHandler<T>&
				     serializationHandler) :
    _socketPtr(socketPtr),
    _serializationHandler(serializationHandler)
  {
  }


  template <typename T, typename U, typename V, std::size_t N, std::size_t N2>
  void PacketSenderFromAsyncTaskHandler<T, U, V, N, N2>::packetError() const
  {
    try
      {
	_socketPtr->send
	  (_serializationHandler
	   .template serialize
	   <Serializable::UniSoulNetworkProtocolSerializable>
	   (std::make_shared<V>
	    (_packetFactory.create(Network::Protocol
				   ::Communication::TCP,
				   Command::Command::NONE,
				   Network::Message
				   ::Server::ERROR))));
      }
    catch (const Exception::SerializationException&) { }
  }
  
  template <typename T, typename U, typename V, std::size_t N, std::size_t N2>
  void PacketSenderFromAsyncTaskHandler<T, U, V, N, N2>
  ::packetSuccess(const std::string& data) const
  {
    try
      {
	_socketPtr->send
	  (_serializationHandler
	   .template serialize
	   <Serializable::UniSoulNetworkProtocolSerializable>
	   (std::make_shared<V>
	    (_packetFactory.create(Network::Protocol
				   ::Communication::TCP,
				   Command::Command::NONE,
				   data.c_str()))));
      }
    catch (const Exception::SerializationException&) { }
  }
}

#endif /* !PACKET_SENDER_FROM_ASYNC_TASK_HANDLER_HPP_ */
