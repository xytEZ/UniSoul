#ifndef MESSAGE_COMMAND_HPP_
# define MESSAGE_COMMAND_HPP_

# include <tuple>
# include <memory>
# include <stdexcept>

# include "AppStateFlag.hh"
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
	std::static_pointer_cast<Network::ITCPSocket<int>>
	  (std::get<1>(tuple).at(std::get<4>(tuple)[1].what))
	  ->send(Serialization::Tool::template serialize
		 <Network::Protocol::UniSoulPacket>
		  (std::get<3>(tuple).create
		   (Network::Protocol::Communication::TCP,
		    std::get<4>(tuple)[1].what == "Server" ?
		    Command::Type::MESSAGE : Command::Type::NONE,
		    std::get<4>(tuple)[2].what.c_str())));
      }
    catch (const std::out_of_range&)
      {
	throw Exception::Network::ErrorWithConnection("Unknown recipient");
      }
    catch (const Exception::Serialization::SerializationFail&)
      {
	throw Exception::Network::ErrorWithConnection("Sending error");
      }
    return App::State::Flag::RUNNING;
  }
}

#endif /* !MESSAGE_COMMAND_HPP_ */
