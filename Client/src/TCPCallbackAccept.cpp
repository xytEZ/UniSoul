#include "ErrorWithConnectionException.hh"
#include "ComplementaryFunction.hpp"
#include "ITCPSocketServer.hpp"
#include "SerializationFailException.hh"
#include "SerializationTool.hpp"
#include "UniSoulNetworkProtocol.hh"
#include "TCPCallbackAccept.hh"

namespace Network
{
  void TCPCallbackAccept::accept(const SocketPtr& socketPtr,
				 Network::IMultiplexer& multiplexer)
  {
    TCPSocketServerPtr	tcpSocketServerPtr;
    TCPSocketPtr	tcpSocketPtr;
    
    tcpSocketServerPtr =
      std::static_pointer_cast<Network::ITCPSocketServer<int, TCPSocketPtr>>
      (socketPtr);
    tcpSocketPtr = std::move(tcpSocketServerPtr->accept());
    setEntryConnectionInfo(tcpSocketPtr);
    multiplexer.addSocket(tcpSocketPtr);
  }

  void TCPCallbackAccept::setEntryConnectionInfo(TCPSocketPtr& tcpSocketPtr)
  {
    constexpr const char	DELIMITER = ';';
    
    try
      {
	RemoteConnectionInfo	remoteConnectionInfo;
       	std::stringstream	ss;
	std::string		data;
	std::string		s;
	
	data = Serialization::Tool::template deserialize
	  <Network::Protocol::UniSoulPacket>(tcpSocketPtr->recv()).data;
	ss << data;
	std::getline(ss, s, DELIMITER);
	remoteConnectionInfo.login = std::move(s);
	ss.str(data.substr(data.find('|') + 1));
	std::getline(ss, s, DELIMITER);
	remoteConnectionInfo.listeningAddress = std::move(s);
	std::getline(ss, s, DELIMITER);
	remoteConnectionInfo.listeningPort =
	  Tool::convert_string_to<unsigned short>(std::move(s));
	tcpSocketPtr->setRemoteConnectionInfo(std::move(remoteConnectionInfo));
      }
    catch (const Exception::Serialization::SerializationFail&)
      {
	throw Exception::Network
	  ::ErrorWithConnection("Error with entry connection");
      }
  }
}
