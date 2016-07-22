#ifndef DECONNECTION_COMMAND_HH_
# define DECONNECTION_COMMAND_HH_

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
    return T();
  }
}

#endif /* !DECONNECTION_COMMAND_HH_ */
