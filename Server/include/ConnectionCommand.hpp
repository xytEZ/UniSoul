#ifndef CONNECTION_COMMAND_HPP_
# define CONNECTION_COMMAND_HPP_

# include <tuple>
# include <boost/any.hpp>
# include "UniSoulSystemWrapper.hh"
# include "Descriptor.hh"
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
  T ConnectionCommand<T, Args...>::execute(Args&... args) const
  {
    std::tuple<Args&...>	tuple = std::forward_as_tuple(args...);
    
    boost::any_cast
      <typename Wrapper::UniSoulSystemWrapper::ClientCheckerManager&>
      (std::get<0>(tuple)->getContent()["ClientCheckerManager"])
      .checkClient(std::get<2>(tuple));
    return true;
  }
}

#endif /* !CONNECTION_COMMAND_HPP_ */
