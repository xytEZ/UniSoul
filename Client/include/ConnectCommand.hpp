#ifndef CONNECT_COMMAND_HPP_
# define CONNECT_COMMAND_HPP_

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
  T ConnectCommand<T, Args...>::execute(Args&...) const
  {
    return App::State::Flag::RUNNING;
  }
}

#endif /* !CONNECT_COMMAND_HPP_ */
