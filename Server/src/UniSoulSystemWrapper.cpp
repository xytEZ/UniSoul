#include "UniSoulSystemWrapper.hh"
#include "PersistentDataFileInteractor.hh"
#include "UniSoulChannelSystem.hh"
#include "TCPBoostSocketServer.hpp"

namespace Wrapper
{
  namespace System
  {
    UniSoulSystemWrapper::UniSoulSystemWrapper(const SocketServerPtr&& socketServerPtr) :
      _components
    {
      { "SocketServer", std::move(socketServerPtr) },
	{ "SocketManager", SocketManager() },
	  { "PersistentDataInteractor", std::make_shared
	      <Persistence::PersistentDataFileInteractor>() },
	    { "ChannelSystem", std::make_shared
		<Communication::UniSoulChannelSystem>() },
	      { "CommandFactory", CommandFactory() },
		{ "CommandExecutor", CommandExecutor() }
    }
    {
    }

    UniSoulSystemWrapper::~UniSoulSystemWrapper() { }
    VariantMap& UniSoulSystemWrapper::getContent() { return _components; }
  }
}
