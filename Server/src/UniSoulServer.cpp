#include "TCPBoostSocketServer.hpp"
#include "PersistentDataFileInteractor.hh"
#include "UniSoulChannelSystem.hh"
#include "UniSoulServer.hh"

namespace App
{
  UniSoulServer::UniSoulServer() :
    _socketServer(std::make_unique<Network::TCPBoostSocketServer<std::unique_ptr<Network::ITCPSocket>>>()),
    _persistentDataInteractor(std::make_unique<Persistence::PersistentDataFileInteractor>()),
    _channelSystem(std::make_unique<Communication::UniSoulChannelSystem>()),
    _socketManager(),
    _commandFactory()
  {
  }
  
  UniSoulServer::~UniSoulServer() { }
  
  bool UniSoulServer::init()
  {
    return true;
  }
  
  bool UniSoulServer::run()
  {
    return true;
  }
  
  bool UniSoulServer::close()
  {
    return true;
  }
}
