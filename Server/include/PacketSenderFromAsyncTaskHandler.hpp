#ifndef PACKET_SENDER_FROM_ASYNC_TASK_HANDLER_HPP_
# define PACKET_SENDER_FROM_ASYNC_TASK_HANDLER_HPP_

# include <cstddef>
# include <memory>
# include <string>

# include "ServerMessage.hh"
# include "SerializationFailException.hh"
# include "CommandType.hh"

namespace Network
{
  template <typename T>
  class ITCPSocket;
  
  template <typename T, typename U, std::size_t N, std::size_t N2>
  class PacketSenderFromAsyncTaskHandler
  {
  private :
    std::shared_ptr<Network::ITCPSocket
    <boost::asio::ip::tcp::socket>>		_socketPtr;
    
    U						_packetFactory;

  public :
    PacketSenderFromAsyncTaskHandler(const std::shared_ptr<Network::ITCPSocket
				     <boost::asio::ip::tcp::socket>>&);
    
    ~PacketSenderFromAsyncTaskHandler() = default;
    void errorPacket() const;
    void successPacket(const std::string&) const;
  };

  template <typename T, typename U, std::size_t N, std::size_t N2>
  PacketSenderFromAsyncTaskHandler<T, U, N, N2>
  ::PacketSenderFromAsyncTaskHandler(const std::shared_ptr<Network::ITCPSocket
				     <boost::asio::ip::tcp::socket>>&
				     socketPtr) :
    _socketPtr(socketPtr)
  {
  }


  template <typename T, typename U, std::size_t N, std::size_t N2>
  void PacketSenderFromAsyncTaskHandler<T, U, N, N2>::errorPacket() const
  {
    try
      {
	_socketPtr->send
	  (Serialization::Tool::template serialize<T>
	    (_packetFactory.create(Network::Protocol::Communication::TCP,
				   Command::Type::NONE,
				   Network::ServerMessage::ERROR)));
      }
    catch (const Exception::Serialization::SerializationFail&) { }
  }
  
  template <typename T, typename U, std::size_t N, std::size_t N2>
  void PacketSenderFromAsyncTaskHandler<T, U, N, N2>
  ::successPacket(const std::string& data) const
  {
    try
      {
	_socketPtr->send
	  (Serialization::Tool::template serialize<T>
	   (_packetFactory.create(Network::Protocol::Communication::TCP,
				  Command::Type::NONE,
				  data.c_str())));
      }
    catch (const Exception::Serialization::SerializationFail&) { }
  }
}

#endif /* !PACKET_SENDER_FROM_ASYNC_TASK_HANDLER_HPP_ */
