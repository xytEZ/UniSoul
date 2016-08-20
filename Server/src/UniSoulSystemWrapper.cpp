#include "PersistentDataFileInteractor.hpp"
#include "File.hpp"
#include "ConnectCommand.hpp"
#include "DisconnectCommand.hpp"
#include "MessageCommand.hpp"
#include "GetUserCommand.hpp"
#include "UniSoulSystemWrapper.hh"

namespace Wrapper
{
  UniSoulSystemWrapper
  ::UniSoulSystemWrapper(const ServerSocketPtr& serverSocketPtr) :
    _components
  {
    { "ServerSocket", std::move(serverSocketPtr) },
      { "SocketManager", SocketManager() },
	{ "ClientChecker", ClientChecker
	    (std::make_unique
	     <Persistence::File::PersistentDataFileInteractor<bool>>
	     (std::make_unique
	      <Persistence::File::File<bool>>
	      ("common/user/users.txt"))) },
	  { "ChatRoomManager", ChatRoomManager() },
	    { "CommandExecutor", CommandExecutor() }
  }
  {
    CommandFactory    cf;
    
    cf.addCommand(Command::Type::CONNECT,
		  std::make_shared<Command::ConnectCommand
		  <Network::ConnectionState,
		  std::unique_ptr<IWrapper<VariantMap>>,
		  std::string,
		  std::string,
		  std::string>>());
    cf.addCommand(Command::Type::DISCONNECT,
		  std::make_shared<Command::DisconnectCommand
		  <Network::ConnectionState,
		  std::unique_ptr<IWrapper<VariantMap>>,
		  std::string,
		  std::string,
		  std::string>>());
    cf.addCommand(Command::Type::MESSAGE,
		  std::make_shared<Command::MessageCommand
		  <Network::ConnectionState,
		  std::unique_ptr<IWrapper<VariantMap>>,
		  std::string,
		  std::string,
		  std::string>>());
    cf.addCommand(Command::Type::GET_USER,
		  std::make_shared<Command::GetUserCommand
		  <Network::ConnectionState,
		  std::unique_ptr<IWrapper<VariantMap>>,
		  std::string,
		  std::string,
		  std::string>>());
    _components["CommandFactory"] = std::move(cf);
  }
  
  VariantMap& UniSoulSystemWrapper::getContent() { return _components; }  
}
