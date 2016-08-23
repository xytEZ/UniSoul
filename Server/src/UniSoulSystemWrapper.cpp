#include "PersistentDataFileInteractor.hpp"
#include "File.hpp"
#include "CreateConnectionCommand.hpp"
#include "MessageCommand.hpp"
#include "GetUserFromCommand.hpp"
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
    
    cf.addCommand(Command::Type::CREATE_CONNECTION,
		  std::make_shared<Command::CreateConnectionCommand
		  <Network::ConnectionState,
		  std::unique_ptr<IWrapper<VariantMap>>,
		  TCPSocketPtr,
		  std::string,
		  std::string>>());
    cf.addCommand(Command::Type::MESSAGE,
		  std::make_shared<Command::MessageCommand
		  <Network::ConnectionState,
		  std::unique_ptr<IWrapper<VariantMap>>,
		  TCPSocketPtr,
		  std::string,
		  std::string>>());
    cf.addCommand(Command::Type::GET_USER_FROM,
		  std::make_shared<Command::GetUserFromCommand
		  <Network::ConnectionState,
		  std::unique_ptr<IWrapper<VariantMap>>,
		  TCPSocketPtr,
		  std::string,
		  std::string>>());
    _components["CommandFactory"] = std::move(cf);
  }
  
  VariantMap& UniSoulSystemWrapper::getContent() { return _components; }  
}
