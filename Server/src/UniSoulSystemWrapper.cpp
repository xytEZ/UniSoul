#include <utility>
#include "PersistentDataFileInteractor.hpp"
#include "File.hpp"
#include "ConnectionCommand.hpp"
#include "DeconnectionCommand.hpp"
#include "UniSoulSystemWrapper.hh"

namespace Wrapper
{
  UniSoulSystemWrapper::UniSoulSystemWrapper(const ServerSocketPtr& serverSocketPtr) :
    _components
  {
    { "ServerSocket", std::move(serverSocketPtr) },
      { "ConnectionManager", ConnectionManager() },
	{ "ClientCheckerManager", ClientCheckerManager
	    (std::make_unique
	     <Persistence::File::PersistentDataFileInteractor<bool>>
	     (std::make_unique
	      <Persistence::File::File<bool>>
	      ("../../other/data/users.txt"))) },
	  { "ChatRoomManager", ChatRoomManager() },
	    { "CommandExecutor", CommandExecutor() }
  }
    {
      CommandFactory	cf;

      cf.addCommand(Command::CONNECTION,
		    std::make_shared<Command::ConnectionCommand
		    <bool,
		    std::unique_ptr<IWrapper<VariantMap>>,
		    std::vector<std::string>,
		    std::string>>());
      cf.addCommand(Command::DECONNECTION,
		    std::make_shared<Command::DeconnectionCommand
		    <bool,
		    std::unique_ptr<IWrapper<VariantMap>>,
		    std::vector<std::string>,
		    std::string>>());
      _components["CommandFactory"] = std::move(cf);
    }

  VariantMap& UniSoulSystemWrapper::getContent() { return _components; }
}
