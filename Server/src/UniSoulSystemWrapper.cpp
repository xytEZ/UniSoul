#include <utility>
#include "PersistentDataFileInteractor.hpp"
#include "File.hpp"
#include "UniSoulSystemWrapper.hh"

namespace Wrapper
{
  namespace System
  {
    UniSoulSystemWrapper::UniSoulSystemWrapper(SocketServerPtr&& socketServerPtr) :
      _components
    {
      { "SocketServer", std::move(socketServerPtr) },
	{ "SocketManager", SocketManager() },
	  { "UserCheckerManager", UserCheckerManager(std::make_unique<Persistence::File::PersistentDataFileInteractor<bool>>(std::make_unique<Persistence::File::File<bool>>(""))) },
	    { "ChannelSystem", ChannelSystem() },
	      { "CommandFactory", CommandFactory() },
		{ "CommandExecutor", CommandExecutor() }
    }
    {
    }

    UniSoulSystemWrapper::~UniSoulSystemWrapper() { }
    VariantMap& UniSoulSystemWrapper::getContent() { return _components; }
  }
}
