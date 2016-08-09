#ifndef DECONNECT_COMMAND_HPP_
# define DECONNECT_COMMAND_HPP_

# include <tuple>

# include "AppStateFlag.hh"
# include "ICommand.hpp"

namespace Command
{
  template <typename T, typename... Args>
  class DeconnectCommand : public ICommand<T, Args...>
  {
  public :
    DeconnectCommand() = default;
    virtual ~DeconnectCommand() = default;
    virtual T execute(Args&...) const;
  };

  template <typename T, typename... Args>
  T DeconnectCommand<T, Args...>::execute(Args&... args) const
  {
    std::tuple<Args&...>	tuple = std::forward_as_tuple(args...);

    if (!std::get<1>(tuple)->close())
      std::get<3>(tuple)
	.append("You're not connected to the server.");
    return App::State::Flag::RUNNING;
  }
}

#endif /* !DECONNECT_COMMAND_HPP_ */
