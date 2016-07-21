#ifndef NETWORK_PACKET_EXCEPTION_HH_
# define NETWORK_PACKET_EXCEPTION_HH_

# include <stdexcept>
# include <string>

namespace Exception
{
  namespace Network
  {
    class PacketStateException : public std::runtime_error
    {
    public :
      PacketStateException(const std::string&);
    };

    class PacketHeaderSizeException : public PacketStateException
    {
    public :
      PacketHeaderSizeException();
    };

    class PacketDataSizeException : public PacketStateException
    {
    public :
      PacketDataSizeException();
    };
  }
}

#endif /* !NETWORK_PACKET_EXCEPTION_HH_ */
