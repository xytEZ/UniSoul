#ifndef CONNECTION_COMMAND_HPP_
# define CONNECTION_COMMAND_HPP_

# include <tuple>
# include "ICommand.hpp"

namespace Command
{
  template <typename T, typename... Args>
  class ConnectionCommand : public ICommand<T, Args...>
  {
  public :
    ConnectionCommand() = default;
    virtual ~ConnectionCommand() = default;
    virtual T execute(Args&...) const;
  };

  template <typename T, typename... Args>
  T ConnectionCommand<T, Args...>::execute(Args&...) const
  {
    //std::tuple<Args&...>	tuple = std::forward_as_tuple(args...);
    
    return T();
  }
}

#endif /* !CONNECTION_COMMAND_HPP_ */
