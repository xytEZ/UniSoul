#include <iostream>
#include <cstring>

#include "SerializationFailException.hh"
#include "ErrorWithConnectionException.hh"
#include "SerializationTool.hpp"
#include "UniSoulNetworkProtocol.hh"
#include "ITCPSocket.hpp"
#include "TCPCallbackRead.hh"

namespace Network
{ 
  void TCPCallbackRead::read(const SocketPtr& socketPtr)
  {
    try
      {
	std::shared_ptr<Network::ITCPSocket<int>>	tcpSocketPtr =
	  std::static_pointer_cast<Network::ITCPSocket<int>>(socketPtr);
	
	Network::Protocol::UniSoulPacket	packet =
	  Serialization::Tool::template deserialize
	  <Network::Protocol::UniSoulPacket>(tcpSocketPtr->recv());

	if (std::strcmp(packet.data, ""))
	  std::cout << tcpSocketPtr->getRecipient() << " : " << packet.data;
	delete[] packet.data;
      }
    catch (const Exception::Serialization::SerializationFail&)
      {
	throw Exception::Network::ErrorWithConnection("Receiving error");
      }
  }
}
