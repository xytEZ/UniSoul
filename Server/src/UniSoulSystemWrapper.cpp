#include "PersistentDataFileInteractor.hpp"
#include "File.hpp"
#include "ConnectCommand.hpp"
#include "DeconnectCommand.hpp"
#include "UniSoulSystemWrapper.hh"

namespace Wrapper
{
  UniSoulSystemWrapper
  ::UniSoulSystemWrapper(const ServerSocketPtr& serverSocketPtr) :
    _components
  {
    { "ServerSocket", std::move(serverSocketPtr) },
      { "ConnectionManager", ConnectionManager() },
	{ "ClientCheckerManager", ClientCheckerManager
	    (std::make_unique
	     <Persistence::File::PersistentDataFileInteractor<bool>>
	     (std::make_unique
	      <Persistence::File::File<bool>>
	      ("../../other/user/users.txt"))) },
	  { "ChatRoomManager", ChatRoomManager() },
	    { "CommandExecutor", CommandExecutor() }
  }
  {
    CommandFactory    cf;
    
    cf.addCommand(Command::Type::CONNECT,
		  std::make_shared<Command::ConnectCommand
		  <Network::ConnectionStateFlag,
		  std::unique_ptr<IWrapper<VariantMap>>,
		  std::vector<std::string>,
		  std::string>>());
    cf.addCommand(Command::Type::DECONNECT,
		  std::make_shared<Command::DeconnectCommand
		  <Network::ConnectionStateFlag,
		  std::unique_ptr<IWrapper<VariantMap>>,
		  std::vector<std::string>,
		  std::string>>());
    _components["CommandFactory"] = std::move(cf);
  }
  
  VariantMap& UniSoulSystemWrapper::getContent() { return _components; }  
}
