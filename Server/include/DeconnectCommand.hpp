#ifndef DECONNECT_COMMAND_HPP_
# define DECONNECT_COMMAND_HPP_

# include <tuple>

# include "ConnectionStateFlag.hh"
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
  T DeconnectCommand<T, Args...>::execute(Args&...) const
  {
    return Network::ConnectionStateFlag::LOG_OUT;
  }
}

#endif /* !DECONNECT_COMMAND_HPP_ */
