#ifndef MESSAGE_COMMAND_HPP_
# define MESSAGE_COMMAND_HPP_

# include <iostream>

# include "ConnectionState.hh"
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
  T MessageCommand<T, Args...>::execute(Args&... args) const
  {
    std::tuple<Args&...>	tuple = std::forward_as_tuple(args...);
    
    std::cout << std::get<1>(tuple)->getRemoteConnectionInfo().login
	      << " : "
	      << std::get<2>(tuple)
	      << std::endl;
    return Network::ConnectionState::NONE;
  }
}

#endif /* !MESSAGE_COMMAND_HPP_ */

