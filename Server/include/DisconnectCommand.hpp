#ifndef DISCONNECT_COMMAND_HPP_
# define DISCONNECT_COMMAND_HPP_

# include <tuple>

# include "ConnectionState.hh"
# include "ServerMessage.hh"
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

    std::get<1>(tuple).push_back(Network::ServerMessage::DISCONNECTION);
    std::cout << "qsdqsd" << std::endl;
    return Network::ConnectionState::DISCONNECTION;
  }
}

#endif /* !DISCONNECT_COMMAND_HPP_ */
