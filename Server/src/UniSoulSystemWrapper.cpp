#include <utility>
#include "PersistentDataFileInteractor.hpp"
#include "File.hpp"
#include "UniSoulSystemWrapper.hh"

namespace Wrapper
{
  UniSoulSystemWrapper::UniSoulSystemWrapper(const ServerSocketPtr& serverSocketPtr) :
    _components
  {
    { "ServerSocket", std::move(serverSocketPtr) },
      { "ConnectionManager", ConnectionManager() },
	{ "ClientCheckerManager", ClientCheckerManager(std::make_unique<Persistence::File::PersistentDataFileInteractor<bool>>(std::make_unique<Persistence::File::File<bool>>(""))) },
	  { "ChatRoomManager", ChatRoomManager() },
	    { "CommandFactory", CommandFactory() },
	      { "CommandExecutor", CommandExecutor() }
  }
    {
    }

  VariantMap& UniSoulSystemWrapper::getContent() { return _components; }
}
