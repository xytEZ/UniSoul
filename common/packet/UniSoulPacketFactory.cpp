#include <cstring>
#include <iostream>
#include "UniSoulPacketFactory.hh"

namespace Factory
{
  Network::Protocol::UniSoulPacket UniSoulPacketFactory
  ::create(Network::Protocol::Communication communication,
	   unsigned short int command,
	   const char *data) const
  {
    Network::Protocol::UniSoulPacket	uniSoulPacket;
    std::size_t				dataLen = std::strlen(data);

    uniSoulPacket.communication = communication;
    uniSoulPacket.command = command;
    uniSoulPacket.data = new char[dataLen];
    std::strncpy(uniSoulPacket.data, data, dataLen);
    uniSoulPacket.data[dataLen] = '\0';
    return uniSoulPacket;
  }
}
