#ifndef STATUS_COMMAND_HPP_
# define STATUS_COMMAND_HPP_

# include <tuple>

# include "ICommand.hpp"

namespace Command
{
  template <typename T, typename... Args>
  class StatusCommand : public ICommand<T, Args...>
  {
  public :
    StatusCommand() = default;
    virtual ~StatusCommand() = default;
    virtual T execute(Args&...) const;
  };

  template <typename T, typename... Args>
  T StatusCommand<T, Args...>::execute(Args&...) const
  {
    return T();
  }
}

#endif /* !STATUS_COMMAND_HPP_ */
