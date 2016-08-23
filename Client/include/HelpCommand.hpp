#ifndef HELP_COMMAND_HPP_
# define HELP_COMMAND_HPP_

# include <tuple>

# include "AppState.hh"
# include "ICommand.hpp"

namespace Command
{
  template <typename T, typename... Args>
  class HelpCommand : public ICommand<T, Args...>
  {
  public :
    HelpCommand() = default;
    virtual ~HelpCommand() = default;
    virtual T execute(Args&...) const;
  };

  template <typename T, typename... Args>
  T HelpCommand<T, Args...>::execute(Args&... args) const
  {
    std::tuple<Args&...>	tuple = std::forward_as_tuple(args...);

    std::get<5>(tuple)
      .append("Commands available :\n\n")
      .append("\t- Help\n")
      .append("\t- Quit\n")
      .append("\t- CreateConnection [LOGIN] [ADDRESS] [PORT]\n")
      .append("\t- Disconnect [LOGIN]\n")
      .append("\t- Message [LOGIN] [CONTENT]\n")
      .append("\t- Status\n")
      .append("\t- GetUserFrom [LOGIN] [LOGIN2]\n");
    return App::State::RUNNING;
  }
}

#endif /* !HELP_COMMAND_HPP_ */
