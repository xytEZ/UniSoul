#ifndef PACKET_SENDER_FROM_ASYNC_TASK_HANDLER_HPP_
# define PACKET_SENDER_FROM_ASYNC_TASK_HANDLER_HPP_

# include <cstddef>
# include <memory>
# include <string>

# include "ServerMessage.hh"
# include "SerializationException.hh"
# include "CommandType.hh"

namespace Network
{
  template <typename T>
  class ITCPSocket;
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
    std::shared_ptr<Network::ITCPSocket
    <boost::asio::ip::tcp::socket>>	_socketPtr;
    
    SerializationHandler<T>		_serializationHandler;
    U					_packetFactory;

  public :
    PacketSenderFromAsyncTaskHandler(const std::shared_ptr<Network::ITCPSocket
				     <boost::asio::ip::tcp::socket>>&,
				     const SerializationHandler<T>&);
    
    ~PacketSenderFromAsyncTaskHandler() = default;
    void packetError() const;
    void packetSuccess(const std::string&) const;
  };

  template <typename T, typename U, typename V, std::size_t N, std::size_t N2>
  PacketSenderFromAsyncTaskHandler<T, U, V, N, N2>
  ::PacketSenderFromAsyncTaskHandler(const std::shared_ptr<Network::ITCPSocket
				     <boost::asio::ip::tcp::socket>>& socketPtr,
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
	  (_serializationHandler.template serialize
	   <Serializable::UniSoulNetworkProtocolSerializable>
	   (std::make_shared<V>
	    (_packetFactory.create(Network::Protocol::Communication::TCP,
				   Command::Type::NONE,
				   Network::Server::Message::ERROR))));
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
	  (_serializationHandler.template serialize
	   <Serializable::UniSoulNetworkProtocolSerializable>
	   (std::make_shared<V>
	    (_packetFactory.create(Network::Protocol::Communication::TCP,
				   Command::Type::NONE,
				   data.c_str()))));
      }
    catch (const Exception::SerializationException&) { }
  }
}

#endif /* !PACKET_SENDER_FROM_ASYNC_TASK_HANDLER_HPP_ */
