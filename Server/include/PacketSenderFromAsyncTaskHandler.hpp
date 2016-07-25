#ifndef PACKET_SENDER_FROM_ASYNC_TASK_HANDLER_HPP_
# define PACKET_SENDER_FROM_ASYNC_TASK_HANDLER_HPP_

# include <cstddef>
# include <memory>
# include <string>
# include "ServerMessage.hh"

namespace Network
{
  template <std::size_t N, std::size_t N2>
  class TCPBoostSocket;
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
    void packetError(bool) const;
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
  void PacketSenderFromAsyncTaskHandler<T, U, V, N, N2>
  ::packetError(bool isFirstTime) const
  {
    if (isFirstTime)
      _socketPtr->setMaintainInstance(false);
    _socketPtr->send
      (_serializationHandler
       .serialize(std::make_shared<V>
		  (_packetFactory.create(Network::Protocol::Communication::TCP,
					 Command::Command::NONE,
					 Network::Message::Server::ERROR))));
  }

  template <typename T, typename U, typename V, std::size_t N, std::size_t N2>
  void PacketSenderFromAsyncTaskHandler<T, U, V, N, N2>
  ::packetSuccess(const std::string& data) const
  {
    _socketPtr->send
      (_serializationHandler
       .serialize(std::make_shared<V>
		  (_packetFactory.create(Network::Protocol::Communication::TCP,
					 Command::Command::NONE,
					 data.c_str()))));
  }
}

#endif /* !PACKET_SENDER_FROM_ASYNC_TASK_HANDLER_HPP_ */
