#ifndef CONNECT_COMMAND_HPP_
# define CONNECT_COMMAND_HPP_

# include <tuple>
# include <boost/any.hpp>

# include "UniSoulSystemWrapper.hh"
# include "ConnectionStateFlag.hh"
# include "ICommand.hpp"

namespace Command
{
  template <typename T, typename... Args>
  class ConnectCommand : public ICommand<T, Args...>
  {
  public :
    ConnectCommand() = default;
    virtual ~ConnectCommand() = default;
    virtual T execute(Args&...) const;
  };

  template <typename T, typename... Args>
  T ConnectCommand<T, Args...>::execute(Args&... args) const
  {
    std::tuple<Args&...>	tuple = std::forward_as_tuple(args...);
    
    return boost::any_cast
      <typename Wrapper::UniSoulSystemWrapper::ClientCheckerManager&>
      (std::get<0>(tuple)->getContent()["ClientCheckerManager"])
      .checkClient(std::get<2>(tuple)) ?
      Network::ConnectionStateFlag::LOG_IN :
      Network::ConnectionStateFlag::LOG_OUT;
  }
}

#endif /* !CONNECT_COMMAND_HPP_ */
