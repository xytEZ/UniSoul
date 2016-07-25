#include <cstring>
#include "UniSoulPacketFactory.hh"

namespace Factory
{
  Network::Protocol::UniSoulPacket UniSoulPacketFactory
  ::create(Network::Protocol::Communication communication,
	   unsigned short int command,
	   const char *data) const
  {
    Network::Protocol::UniSoulPacket	uniSoulPacket;

    uniSoulPacket.header.header_size
      = sizeof(communication) + sizeof(command);      
    uniSoulPacket.header.communication = communication;
    uniSoulPacket.header.command = command;
    uniSoulPacket.data.data_size = std::strlen(data);
    std::strncpy(reinterpret_cast<char *>(uniSoulPacket.data.data),
		 data,
		 uniSoulPacket.data.data_size);
    return uniSoulPacket;
  }
}
