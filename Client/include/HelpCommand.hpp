#ifndef HELP_COMMAND_HPP_
# define HELP_COMMAND_HPP_

# include <iostream>

# include <tuple>

# include "AppStateFlag.hh"
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

    std::get<1>(tuple)
      .append("Commands available :\n\n")
      .append("\t- Help\n")
      .append("\t- Quit\n")
      .append("\t- Connect\n")
      .append("\t- Deconnect");
    return App::State::RUNNING;
  }
}

#endif /* !HELP_COMMAND_HPP_ */
