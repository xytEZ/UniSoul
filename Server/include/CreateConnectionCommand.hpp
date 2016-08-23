#ifndef CREATE_CONNECTION_COMMAND_HPP_
# define CREATE_CONNECTION_COMMAND_HPP_

# include <tuple>
# include <boost/any.hpp>

# include "ITCPSocket.hpp"
# include "UniSoulSystemWrapper.hh"
# include "ConnectionState.hh"
# include "ServerMessage.hh"
# include "ICommand.hpp"

namespace Command
{
  template <typename T, typename... Args>
  class CreateConnectionCommand : public ICommand<T, Args...>
  {
  public :
    CreateConnectionCommand() = default;
    virtual ~CreateConnectionCommand() = default;
    virtual T execute(Args&...) const;    
  };

  template <typename T, typename... Args>
  T CreateConnectionCommand<T, Args...>::execute(Args&... args) const
  {
    std::tuple<Args&...>	tuple = std::forward_as_tuple(args...);
    std::string			identifier;
    std::string			login;
    Network::ConnectionState	state;

    identifier = std::get<2>(tuple).substr(0, std::get<2>(tuple).find('|'));
    login = identifier.substr(0, identifier.find_first_of(';'));
    state = boost::any_cast
      <typename Wrapper::UniSoulSystemWrapper::ClientChecker&>
      (std::get<0>(tuple)->getContent()["ClientChecker"]).check(identifier)
      && !static_cast<bool>
      (boost::any_cast
       <typename Wrapper::UniSoulSystemWrapper::SocketManager&>
       (std::get<0>(tuple)->getContent()["SocketManager"])
       .findSocketPtrIf([&login]
			(const std::shared_ptr
			 <Network::ISocket<boost::asio::ip::tcp::socket>>&
			 socketPtr) -> bool
			{
			  std::shared_ptr
			    <Network::ITCPSocket
			     <boost::asio::ip::tcp::socket>>	tcpSocketPtr =
			    std::static_pointer_cast
			    <Network::ITCPSocket
			     <boost::asio::ip::tcp::socket>>(socketPtr);
			  
			  return tcpSocketPtr
			    && tcpSocketPtr->getRemoteConnectionInfo().login
			    == login;
			})) ?
      Network::ConnectionState::ACCEPTED_CONNECTION :
      Network::ConnectionState::REFUSED_CONNECTION;
    return state;
  }
}

#endif /* !CREATE_CONNECTION_COMMAND_HPP_ */
