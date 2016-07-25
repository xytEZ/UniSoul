#ifndef UNI_SOUL_PACKET_STATE_CHECKER_HH_
# define UNI_SOUL_PACKET_STATE_CHECKER_HH_

# include "UniSoulNetworkProtocol.hh"
# include "IPacketStateChecker.hpp"

namespace Network
{
  class UniSoulPacketStateChecker :
    public IPacketStateChecker<Network::Protocol::UniSoulPacket>
  {
  private :
    Network::Protocol::UniSoulPacket	_uniSoulPacket;

  public :
    UniSoulPacketStateChecker() = default;
    UniSoulPacketStateChecker(const Network::Protocol::UniSoulPacket&);
    virtual ~UniSoulPacketStateChecker() = default;
    virtual void setPacket(const Network::Protocol::UniSoulPacket&);
    virtual bool checkPacket() const;

  private :
    void checkPacketHeaderSize() const;
    void checkPacketDataSize() const;
  };
}

#endif /* !UNI_SOUL_PACKET_STATE_CHECKER_HH_ */
