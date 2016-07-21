#ifndef UNI_SOUL_NETWORK_PROTOCOL_SERIALIZABLE_HH_
# define UNI_SOUL_NETWORK_PROTOCOL_SERIALIZABLE_HH_

# include <boost/serialization/export.hpp>
# include "UniSoulNetworkProtocol.hh"
# include "ASerializable.hpp"

namespace Serializable
{
  class UniSoulNetworkProtocolSerializable :
    public ASerializable<UniSoulPacket>
  {
  private :
    friend class boost::serialization::access;
    
  public :
    UniSoulNetworkProtocolSerializable() = default;
    UniSoulNetworkProtocolSerializable(const UniSoulPacket&);
    virtual ~UniSoulNetworkProtocolSerializable() = default;
    virtual const UniSoulPacket& getSerializableComponent() const;
    virtual void setSerializableComponent(const UniSoulPacket&);

  private :
    template <class Archive>
    void serialize(Archive&, const unsigned int);
  };
}

#endif /* !UNI_SOUL_NETWORK_PROTOCOL_SERIALIZABLE_HH_ */
