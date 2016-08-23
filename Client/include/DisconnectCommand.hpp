#ifndef DISCONNECT_COMMAND_HPP_
# define DISCONNECT_COMMAND_HPP_

# include <tuple>

# include "AppState.hh"
# include "CommandType.hh"
# include "ErrorWithConnectionException.hh"
# include "ITCPSocket.hpp"
# include "SerializationTool.hpp"
# include "ICommand.hpp"

namespace Command
{
  template <typename T, typename... Args>
  class DisconnectCommand : public ICommand<T, Args...>
  {
  public :
    DisconnectCommand() = default;
    virtual ~DisconnectCommand() = default;
    virtual T execute(Args&...) const;
  };

  template <typename T, typename... Args>
  T DisconnectCommand<T, Args...>::execute(Args&... args) const
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
			       && tcpSocketPtr->getRemoteConnectionInfo().login
			       == std::get<4>(tuple)[1].what)
			     {
			       std::get<0>(tuple)
				 ->closeSocket(socketCallbackPtr->socketPtr);
			       return true;
			     }
			   return false;
			 })
	    == std::get<0>(tuple)->getSocketCallbacksPtr().cend())
	  throw Exception::Network::ErrorWithConnection
	    ("Unknown remote connection\n");
      }
    catch (const Exception::Serialization::SerializationFail&)
      {
	throw Exception::Network::ErrorWithConnection("Sending error\n");
      }
    catch (const std::system_error& e)
      {
	if (!std::strcmp(e.what(), "Broken pipe"))
	  throw Exception::Network
	    ::ErrorWithConnection("Error with remote connection\n");
	throw Exception::Network::ErrorWithConnection(std::string(e.what())
						      + '\n');
      }
    return App::State::RUNNING;
  }
}

#endif /* !DISCONNECT_COMMAND_HPP_ */
