#include "RemoteConnectionInfo.hh"

namespace Network
{
  RemoteConnectionInfo
  ::RemoteConnectionInfo(const std::string& login,
			 const std::string& listeningAddress,
			 unsigned short listeningPort) 
  {
    this->login = login;
    this->listeningAddress = listeningAddress;
    this->listeningPort = listeningPort;
  }
}
