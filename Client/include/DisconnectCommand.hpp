#ifndef DISCONNECT_COMMAND_HPP_
# define DISCONNECT_COMMAND_HPP_

# include <tuple>

# include "AppStateFlag.hh"
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
	std::get<1>(tuple)->send
	  (Serialization::Tool::template serialize
	   <Network::Protocol::UniSoulPacket>
	   (std::get<3>(tuple).create
	    (Network::Protocol::Communication::TCP,
	     Command::Type::DISCONNECT,
	     "")));
      }
    catch (const Exception::Serialization::SerializationFail&) { }
    return App::State::Flag::RUNNING;
  }
}

#endif /* !DISCONNECT_COMMAND_HPP_ */
