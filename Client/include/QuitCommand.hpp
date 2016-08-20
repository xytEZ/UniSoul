#ifndef QUIT_COMMAND_HPP_
# define QUIT_COMMAND_HPP_

# include "AppState.hh"
# include "ICommand.hpp"

namespace Command
{
  template <typename T, typename... Args>
  class QuitCommand : public ICommand<T, Args...>
  {
  public :
    QuitCommand() = default;
    virtual ~QuitCommand() = default;
    virtual T execute(Args&...) const;
  };

  template <typename T, typename... Args>
  T QuitCommand<T, Args...>::execute(Args&...) const
  {
    return App::State::STOP;
  }
}

#endif /* !QUIT_COMMAND_HPP_ */
