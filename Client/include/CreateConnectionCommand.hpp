#ifndef CREATE_CONNECTION_COMMAND_HPP_
# define CREATE_CONNECTION_COMMAND_HPP_

# include <tuple>
# include <cstring>

# include "RemoteConnectionInfo.hh"
# include "CommandType.hh"
# include "SerializationTool.hpp"
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
    initConnection(tuple);
    return App::State::RUNNING;
  }
  
  template <typename T, typename... Args>
  void CreateConnectionCommand<T, Args...>
  ::inspectQuery(std::tuple<Args&...>& tuple) const
  {
    if (std::find_if(std::get<0>(tuple)->getSocketCallbacksPtr().cbegin(),
		     std::get<0>(tuple)->getSocketCallbacksPtr().cend(),
		     [&tuple]
		     (const Network::IMultiplexer::SocketCallbackPtr&
		      socketCallbackPtr) -> bool
		     {
		       std::shared_ptr
			 <Network::ITCPSocket<int>>	tcpSocketPtr =
			 std::dynamic_pointer_cast<Network::ITCPSocket<int>>
			 (socketCallbackPtr->socketPtr);
		       
		       return tcpSocketPtr
			 && (tcpSocketPtr->getRemoteConnectionInfo().login
			     == std::get<4>(tuple)[1].what
			     || (tcpSocketPtr->getAddress()
				 == std::get<4>(tuple)[2].what
				 && tcpSocketPtr->getPort()
				 == Tool::convert_string_to<unsigned short>
				 (std::get<4>(tuple)[3].what)));
		     }) != std::get<0>(tuple)->getSocketCallbacksPtr().cend())
      throw Exception::Network::ErrorWithConnection
	("Login or endpoint already in use\n");
  }
  
  template <typename T, typename... Args>
  void CreateConnectionCommand<T, Args...>
  ::initConnection(std::tuple<Args&...>& tuple) const
  {
    std::shared_ptr
      <Network::ITCPSocketClient<int>>	tcpSocketPtr;
    
#if defined(linux) || defined(__linux)
    tcpSocketPtr = std::make_shared<Network::TCPSocketClientLinux<>>
      (std::get<4>(tuple)[2].what,
       Tool::convert_string_to<unsigned short>(std::get<4>(tuple)[3].what),
       Network::RemoteConnectionInfo
       (std::get<4>(tuple)[1].what,
	std::get<4>(tuple)[2].what,
	Tool::convert_string_to<unsigned short>(std::get<4>(tuple)[3].what)));
#else
# error "Can't create the socket. Unknown operating system."
#endif

    try
      {
	std::vector<std::string>	data(std::get<2>(tuple).find());
	std::string			identifier
	  (std::accumulate(data.cbegin(), data.cend(), std::string()));
	
	tcpSocketPtr->open();
	tcpSocketPtr->connect();
	tcpSocketPtr->send
	  (Serialization::Tool::template serialize
	   <Network::Protocol::UniSoulPacket>
	   (std::get<3>(tuple).create
	    (Network::Protocol::Communication::TCP,
	     Command::Type::CREATE_CONNECTION,
	     std::string(identifier
			 + '|'
			 + std::get<1>(tuple)->getAddress()
			 + ';'
			 + std::to_string(std::get<1>(tuple)->getPort()))
			 .c_str())));
	std::get<0>(tuple)->addSocket(tcpSocketPtr);
      }
    catch (const std::ifstream::failure&)
      {
	throw Exception::Network
	  ::ErrorWithConnection("Untraceable identifier\n");
      }
    catch (const Exception::Serialization::SerializationFail&)
      {
	throw Exception::Network
	  ::ErrorWithConnection("Authentification sending error\n");
      }
    catch (const std::system_error& e)
      {
	if (!std::strcmp(e.what(), "Broken pipe"))
	  throw Exception::Network
	    ::ErrorWithConnection("Error with remote connection\n");
	throw Exception::Network::ErrorWithConnection(std::string(e.what())
						      + '\n');
      }
  }
}

#endif /* :CREATE_CONNECTION_COMMAND_HPP_ */
