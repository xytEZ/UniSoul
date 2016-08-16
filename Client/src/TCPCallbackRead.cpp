#include <iostream>

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
	Network::Protocol::UniSoulPacket	packet =
	  Serialization::Tool::template deserialize
	  <Network::Protocol::UniSoulPacket>
	  (std::static_pointer_cast<Network::ITCPSocket<int>>(socketPtr)
	   ->recv());

	std::cout << packet.data;
	delete[] packet.data;
      }
    catch (const Exception::Serialization::SerializationFail&)
      {
	throw Exception::Network::ErrorWithConnection("Receiving error");
      }
  }
}
