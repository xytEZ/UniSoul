#ifndef CREATE_CONNECTION_COMMAND_HPP_
# define CREATE_CONNECTION_COMMAND_HPP_

# include <tuple>
# include <cstring>

# include "ComplementaryFunction.hpp"
# include "ErrorWithConnectionException.hh"
# include "ITCPSocketClient.hpp"
# include "IMultiplexer.hh"
# include "AppState.hh"
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

  private :
    void inspectQuery(std::tuple<Args&...>&) const;
    void initConnection(std::tuple<Args&...>&) const;
  };
  
  template <typename T, typename... Args>
  T CreateConnectionCommand<T, Args...>::execute(Args&... args) const
  {
    std::tuple<Args&...>	tuple = std::forward_as_tuple(args...);

    inspectQuery(tuple);
    try
      {
	initConnection(tuple);
      }
    catch (const std::system_error& e)
      {
	if (!std::strcmp(e.what(), "Broken pipe"))
	  throw Exception::Network
	    ::ErrorWithConnection("Error with remote connection");
	throw Exception::Network::ErrorWithConnection(e.what());
      }
    return App::State::RUNNING;
  }
  
  template <typename T, typename... Args>
  void CreateConnectionCommand<T, Args...>
  ::inspectQuery(std::tuple<Args&...>& tuple) const
  {
    if (std::get<1>(tuple)->getRecipient() == std::get<4>(tuple)[1].what
	|| (std::get<1>(tuple)->getAddress() == std::get<4>(tuple)[2].what
	    && std::get<1>(tuple)->getPort()
	    == Tool::convert_string_to<unsigned short>
	    (std::get<4>(tuple)[3].what)))
      throw Exception::Network
	::ErrorWithConnection("The login or IP address is reserved");
    std::for_each(std::get<0>(tuple)->getSocketCallbacksPtr().cbegin(),
		  std::get<0>(tuple)->getSocketCallbacksPtr().cend(),
		  [&tuple]
		  (const Network::IMultiplexer::SocketCallbackPtr&
		   socketCallbackPtr) -> void
		  {
		    std::shared_ptr<Network::ITCPSocket<int>>	tcpSocketPtr =
		      std::dynamic_pointer_cast<Network::ITCPSocket<int>>
		      (socketCallbackPtr->socketPtr);
		    
		    if (tcpSocketPtr
			&& (tcpSocketPtr->getRecipient()
			    == std::get<4>(tuple)[1].what
			    || (tcpSocketPtr->getAddress()
				== std::get<4>(tuple)[2].what
				&& tcpSocketPtr->getPort()
				== Tool::convert_string_to<unsigned short>
				(std::get<4>(tuple)[3].what))))
		      throw Exception::Network::ErrorWithConnection
			("The login or IP address is already used");
		  });
  }
  
  template <typename T, typename... Args>
  void CreateConnectionCommand<T, Args...>
  ::initConnection(std::tuple<Args&...>& tuple) const
  {
    std::shared_ptr
      <Network::ITCPSocketClient<int>>	tcpSocketPtr;
    
#if defined(linux) || defined(__linux)
    tcpSocketPtr = std::make_shared<Network::TCPSocketClientLinux<>>
      (std::get<4>(tuple)[1].what,
       Tool::convert_string_to<unsigned short>(std::get<4>(tuple)[2].what));
#else
# error "Can't create the connection. Unknown operating system."
#endif
    
    tcpSocketPtr->open();
    tcpSocketPtr->connect();
    std::get<0>(tuple)->addSocket(tcpSocketPtr);
  }
}

#endif /* :CREATE_CONNECTION_COMMAND_HPP_ */
