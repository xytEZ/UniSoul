#ifndef CONNECT_COMMAND_HPP_
# define CONNECT_COMMAND_HPP_

# include <tuple>

# include "AppStateFlag.hh"
# include "CommandType.hh"
# include "ErrorWithConnectionException.hh"
# include "ITCPSocketClient.hpp"
# include "SerializationTool.hpp"
# include "ICommand.hpp"

namespace Command
{
  template <typename T, typename... Args>
  class ConnectCommand : public ICommand<T, Args...>
  {
  public :
    ConnectCommand() = default;
    virtual ~ConnectCommand() = default;
    virtual T execute(Args&...) const;
  };

  template <typename T, typename... Args>
  T ConnectCommand<T, Args...>::execute(Args&... args) const
  {
    std::tuple<Args&...>	tuple(std::forward_as_tuple(args...));
    
    try
      {
	std::string		identifier(std::get<2>(tuple).find()[0]);
	
	std::get<1>(tuple)["Server"]->open();
	std::static_pointer_cast<Network::ITCPSocketClient<int>>
	  (std::get<1>(tuple)["Server"])->connect();
	std::get<0>(tuple)->addSocket(std::get<1>(tuple)["Server"]);
	try
	  {
	    std::static_pointer_cast<Network::ITCPSocket<int>>
	      (std::get<1>(tuple)["Server"])
	      ->send(Serialization::Tool::template serialize
		     <Network::Protocol::UniSoulPacket>
		     (std::get<3>(tuple).create
		       (Network::Protocol::Communication::TCP,
			Command::Type::CONNECT,
			identifier.c_str())));
	  }
	catch (const Exception::Serialization::SerializationFail&)
	  {
	    throw Exception::Network
	      ::ErrorWithConnection("Authentification sending error");
	  }
      }
    catch (const std::ifstream::failure&)
      {
	throw Exception::Network
	  ::ErrorWithConnection("Untraceable identifier");
      }
    return App::State::Flag::RUNNING;
  }
}

#endif /* !CONNECT_COMMAND_HPP_ */
