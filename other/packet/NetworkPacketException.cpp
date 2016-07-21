#include "NetworkPacketException.hh"

namespace Exception
{
  namespace Network
  {
    PacketStateException::PacketStateException(const std::string& what_arg) :
      std::runtime_error(what_arg)
    {
    }

    PacketHeaderSizeException::PacketHeaderSizeException() :
      PacketStateException("Packet header size is incoherent.")
    {
    }

    PacketDataSizeException::PacketDataSizeException() :
      PacketStateException("Packet data size is incoherent.")
    {
    }
  }
}
