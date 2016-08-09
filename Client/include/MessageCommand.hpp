#ifndef MESSAGE_COMMAND_HPP_
# define MESSAGE_COMMAND_HPP_

# include <tuple>

# include "AppStateFlag.hh"
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
  T MessageCommand<T, Args...>::execute(Args&...) const
  {
    //std::tuple<Args&...>	tuple = std::forward_as_tuple(args...);

    return App::State::Flag::RUNNING;
  }
}

#endif /* !MESSAGE_COMMAND_HPP_ */
