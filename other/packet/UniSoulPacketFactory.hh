#ifndef UNI_SOUL_PACKET_FACTORY_HH_
# define UNI_SOUL_PACKET_FACTORY_HH_

# include "UniSoulNetworkProtocol.hh"

namespace Factory
{
  class UniSoulPacketFactory
  {
  public :
    UniSoulPacketFactory() = default;
    ~UniSoulPacketFactory() = default;
    
  public :
    UniSoulPacket create(Network::Protocol::Communication,
			 Network::Protocol::Request,
			 const char *) const;
  };
}

#endif /* !UNI_SOUL_PACKET_FACTORY_HH_ */
