#ifndef GET_USER_FROM_COMMAND_HPP_
# define GET_USER_FROM_COMMAND_HPP_

# include <tuple>
# include <algorithm>
# include <boost/any.hpp>

# include "UniSoulSystemWrapper.hh"
# include "ConnectionState.hh"
# include "ITCPSocket.hpp"
# include "ICommand.hpp"

namespace Command
{
  template <typename T, typename... Args>
  class GetUserFromCommand : public ICommand<T, Args...>
  {
  public :
    GetUserFromCommand() = default;
    virtual ~GetUserFromCommand() = default;
    virtual T execute(Args&...) const;
  };

  template <typename T, typename... Args>
  T GetUserFromCommand<T, Args...>::execute(Args&... args) const
  {
    std::tuple<Args&...>		tuple = std::forward_as_tuple(args...);

    
    std::shared_ptr
      <Network::ITCPSocket
       <boost::asio::ip::tcp::socket>>	tcpSocketPtr =
      std::static_pointer_cast
      <Network::ITCPSocket<boost::asio::ip::tcp::socket>>
      ((boost::any_cast
	<typename Wrapper::UniSoulSystemWrapper::SocketManager&>
	(std::get<0>(tuple)->getContent()["SocketManager"])
	.findSocketPtrIf([&tuple]
			 (const std::shared_ptr
			  <Network::ISocket<boost::asio::ip::tcp::socket>>&
			  socketPtr) -> bool
			 { 
			   return std::static_pointer_cast
			     <Network::ITCPSocket
			      <boost::asio::ip::tcp::socket>>
			     (socketPtr)->getRemoteConnectionInfo().login
			     == std::get<2>(tuple);
			 })));
    
    if (tcpSocketPtr)
      std::get<3>(tuple)
	.append(tcpSocketPtr->getRemoteConnectionInfo().login)
	.append(" (")
	.append(tcpSocketPtr->getRemoteConnectionInfo().listeningAddress)
	.append(" ")
	.append(std::to_string(tcpSocketPtr->getRemoteConnectionInfo()
			       .listeningPort))
	.append(")");
    else
      std::get<3>(tuple)
	.append("User not found");
    return Network::ConnectionState::NONE;
  }
}

#endif /* !GET_USER_FROM_COMMAND_HPP_ */
