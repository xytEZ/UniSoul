#ifndef UNI_SOUL_PACKET_STATE_CHECKER_HH_
# define UNI_SOUL_PACKET_STATE_CHECKER_HH_

# include "UniSoulNetworkProtocol.hh"
# include "IPacketStateChecker.hpp"

namespace Network
{
  class UniSoulPacketStateChecker : public IPacketStateChecker<UniSoulPacket>
  {
  private :
    UniSoulPacket	_uniSoulPacket;

  public :
    UniSoulPacketStateChecker() = default;
    UniSoulPacketStateChecker(const UniSoulPacket&);
    virtual ~UniSoulPacketStateChecker() = default;
    virtual void setPacket(const UniSoulPacket&);
    virtual bool checkPacket() const;

  private :
    void checkPacketHeaderSize() const;
    void checkPacketDataSize() const;
  };
}

#endif /* !UNI_SOUL_PACKET_STATE_CHECKER_HH_ */
