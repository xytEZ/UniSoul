#ifndef CONNECT_COMMAND_HPP_
# define CONNECT_COMMAND_HPP_

# include <tuple>
# include <cstring>

# include "AppState.hh"
# include "CommandType.hh"
# include "ErrorWithConnectionException.hh"
# include "ITCPSocketClient.hpp"
# include "SerializationTool.hpp"
# include "IMultiplexer.hh"
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

  private :
    void inspectQuery(std::tuple<Args&...>&) const;
    void initConnection(std::tuple<Args&...>&) const;
  };

  template <typename T, typename... Args>
  T ConnectCommand<T, Args...>::execute(Args&... args) const
  {
    std::tuple<Args&...>	tuple(std::forward_as_tuple(args...));
    
    inspectQuery(tuple);
    initConnection(tuple);
    return App::State::RUNNING;
  }

  template <typename T, typename... Args>
  void
  ConnectCommand<T, Args...>::inspectQuery(std::tuple<Args&...>& tuple) const
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
		       
		       return tcpSocketPtr ?
			 (tcpSocketPtr->getRecipient()
			  == std::get<1>(tuple)->getRecipient())
			 : false;
		     }) != std::get<0>(tuple)->getSocketCallbacksPtr().cend())
      throw Exception::Network
	::ErrorWithConnection("Already connected to the server");
  }

  template <typename T, typename... Args>
  void
  ConnectCommand<T, Args...>::initConnection(std::tuple<Args&...>& tuple) const
  {
    try
      {
	std::string		identifier(std::get<2>(tuple).find()[0]);

	std::get<1>(tuple)->open();
	std::get<1>(tuple)->connect();
	std::get<0>(tuple)->addSocket(std::get<1>(tuple));
	std::get<1>(tuple)->send
	  (Serialization::Tool::template serialize
	   <Network::Protocol::UniSoulPacket>
	   (std::get<3>(tuple).create
	    (Network::Protocol::Communication::TCP,
	     Command::Type::CONNECT,
	     identifier.c_str())));
      }
    catch (const std::ifstream::failure&)
      {
	throw Exception::Network
	  ::ErrorWithConnection("Untraceable identifier");
      }
    catch (const Exception::Serialization::SerializationFail&)
      {
	throw Exception::Network
	  ::ErrorWithConnection("Authentification sending error");
      }
    catch (const std::system_error& e)
      {
	if (!std::strcmp(e.what(), "Broken pipe"))
	  throw Exception::Network
	    ::ErrorWithConnection("Error with remote connection");
	throw Exception::Network::ErrorWithConnection(e.what());
      }
  }
}

#endif /* !CONNECT_COMMAND_HPP_ */
