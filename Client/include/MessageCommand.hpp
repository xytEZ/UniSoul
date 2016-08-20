#ifndef MESSAGE_COMMAND_HPP_
# define MESSAGE_COMMAND_HPP_

# include <tuple>
# include <memory>
# include <cstring>

# include "AppState.hh"
# include "ErrorWithConnectionException.hh"
# include "CommandType.hh"
# include "ICommand.hpp"

namespace Command
{
  template <typename T, typename... Args>
  class MessageCommand : public ICommand<T, Args...>
  {
  public :
    MessageCommand() = default;
    virtual ~MessageCommand() = default;
    virtual T execute(Args&...) const;
  };

  template <typename T, typename... Args>
  T MessageCommand<T, Args...>::execute(Args&... args) const
  {
    std::tuple<Args&...>	tuple = std::forward_as_tuple(args...);
    
    try
      {
	if (std::find_if(std::get<0>(tuple)->getSocketCallbacksPtr().cbegin(),
			 std::get<0>(tuple)->getSocketCallbacksPtr().cend(),
			 [&tuple]
			 (const Network::IMultiplexer::SocketCallbackPtr&
			  socketCallbackPtr) -> bool
			 {
			   std::shared_ptr
			     <Network::ITCPSocket<int>>	tcpSocketPtr =
			     std::dynamic_pointer_cast
			     <Network::ITCPSocket<int>>
			     (socketCallbackPtr->socketPtr);
			   
			   if (tcpSocketPtr
			       && tcpSocketPtr->getRecipient()
			       == std::get<4>(tuple)[1].what)
			     {
			       tcpSocketPtr->send
				 (Serialization::Tool::template serialize
				  <Network::Protocol::UniSoulPacket>
				  (std::get<3>(tuple).create
				   (Network::Protocol::Communication::TCP,
				    std::get<4>(tuple)[1].what == "Server" ?
				    Command::Type::MESSAGE
				    : Command::Type::NONE,
				    std::get<4>(tuple)[2].what.c_str())));
			       return true;
			     }
			   return false;
			 })
	    == std::get<0>(tuple)->getSocketCallbacksPtr().cend())
	  throw Exception::Network::ErrorWithConnection("Unknown recipient");
      }
    catch (const Exception::Serialization::SerializationFail&)
      {
	throw Exception::Network::ErrorWithConnection("Sending error");
      }
    catch (const std::system_error& e)
      {
	if (!std::strcmp(e.what(), "Broken pipe"))
	  throw Exception::Network
	    ::ErrorWithConnection("Error with remote connection");
	throw Exception::Network::ErrorWithConnection(e.what());
      }
    return App::State::RUNNING;
  }
}

#endif /* !MESSAGE_COMMAND_HPP_ */
