#ifndef UNI_SOUL_NETWORK_PROTOCOL_SERIALIZABLE_HH_
# define UNI_SOUL_NETWORK_PROTOCOL_SERIALIZABLE_HH_

# include "UniSoulNetworkProtocol.hh"
# include "ASerializable.hpp"

namespace Serializable
{
  class UniSoulNetworkProtocolSerializable :
    public ASerializable<Network::Protocol::UniSoulPacket>
  {
  private :
    friend class boost::serialization::access;
    
  public :
    UniSoulNetworkProtocolSerializable() = default;
    UniSoulNetworkProtocolSerializable(const Network::Protocol
				       ::UniSoulPacket&);
    virtual ~UniSoulNetworkProtocolSerializable();
    virtual const Network::Protocol::UniSoulPacket&
    getSerializableComponent() const;
    
    virtual void setSerializableComponent(const Network::Protocol
					  ::UniSoulPacket&);
    
  private :
    template <class Archive>
    void serialize(Archive&, const unsigned int);
  };
}

#endif /* !UNI_SOUL_NETWORK_PROTOCOL_SERIALIZABLE_HH_ */
