#ifndef GET_USER_COMMAND_HPP_
# define GET_USER_COMMAND_HPP_

# include <tuple>
# include <cstring>

# include "AppState.hh"
# include "SerializationTool.hpp"
# include "ErrorWithConnectionException.hh"
# include "CommandType.hh"
# include "ICommand.hpp"

namespace Command
{
  template <typename T, typename... Args>
  class GetUserCommand : public ICommand<T, Args...>
  {
  public :
    GetUserCommand() = default;
    virtual ~GetUserCommand() = default;
    virtual T execute(Args&...) const;
  };

  template <typename T, typename... Args>
  T GetUserCommand<T, Args...>::execute(Args&... args) const
  {
    std::tuple<Args&...>	tuple = std::forward_as_tuple(args...);
    
    try
      {
	std::get<1>(tuple)->send
	  (Serialization::Tool::template serialize
	   <Network::Protocol::UniSoulPacket>
	   (std::get<3>(tuple).create
	    (Network::Protocol::Communication::TCP,
	     Command::Type::GET_USER,
	     std::get<4>(tuple)[1].what.c_str())));
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

#endif /* GET_USER_COMMAND_HPP_ */
