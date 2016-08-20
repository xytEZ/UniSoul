#ifndef CONNECT_COMMAND_HPP_
# define CONNECT_COMMAND_HPP_

# include <tuple>
# include <boost/any.hpp>

# include "UniSoulSystemWrapper.hh"
# include "ConnectionState.hh"
# include "ServerMessage.hh"
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
    std::tuple<Args&...>	tuple = std::forward_as_tuple(args...);
    Network::ConnectionState	state =
      boost::any_cast
      <typename Wrapper::UniSoulSystemWrapper::ClientChecker&>
      (std::get<0>(tuple)->getContent()["ClientChecker"])
      .check(std::get<2>(tuple)) ?
      Network::ConnectionState::ACCEPTED_CONNECTION :
      Network::ConnectionState::REFUSED_CONNECTION;
    
    std::get<3>(tuple) = 
      (state == Network::ConnectionState::ACCEPTED_CONNECTION ?
       Network::ServerMessage::ACCEPTED_CONNECTION :
       Network::ServerMessage::REFUSED_CONNECTION);
    return state;
  }
}

#endif /* !CONNECT_COMMAND_HPP_ */
