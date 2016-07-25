#ifndef DECONNECTION_COMMAND_HPP_
# define DECONNECTION_COMMAND_HPP_

# include <tuple>
# include "ICommand.hpp"

namespace Command
{
  template <typename T, typename... Args>
  class DeconnectionCommand : public ICommand<T, Args...>    
  {
  public :
    DeconnectionCommand() = default;
    virtual ~DeconnectionCommand() = default;
    virtual T execute(Args&...) const;
  };

  template <typename T, typename... Args>
  T DeconnectionCommand<T, Args...>::execute(Args&...) const
  {
    return true;
  }
}

#endif /* !DECONNECTION_COMMAND_HPP_ */
