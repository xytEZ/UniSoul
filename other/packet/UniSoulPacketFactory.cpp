#include <cstring>
#include "UniSoulPacketFactory.hh"

namespace Factory
{
  UniSoulPacket UniSoulPacketFactory
  ::create(Network::Protocol::Communication communication,
	   Network::Protocol::Request request,
	   const char *data) const
  {
    UniSoulPacket	uniSoulPacket;

    uniSoulPacket.header.header_size
      = sizeof(communication) + sizeof(request);      
    uniSoulPacket.header.communication = communication;
    uniSoulPacket.header.request = request;
    uniSoulPacket.data.data_size = std::strlen(data);
    std::strncpy(reinterpret_cast<char *>(uniSoulPacket.data.data),
		 data,
		 uniSoulPacket.data.data_size);
    return uniSoulPacket;
  }
}
