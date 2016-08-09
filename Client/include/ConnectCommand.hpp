#ifndef CONNECT_COMMAND_HPP_
# define CONNECT_COMMAND_HPP_

# include <tuple>

# include "AppStateFlag.hh"
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

    if (!(std::get<1>(tuple)->open()
	  && std::get<1>(tuple)->connect()))
      std::get<3>(tuple).append("Already connected to the server.");
    std::get<0>(tuple)->addSocket(std::get<1>(tuple));
    return App::State::Flag::RUNNING;
  }
}

#endif /* !CONNECT_COMMAND_HPP_ */
