#include <boost/serialization/base_object.hpp>
#include "UniSoulNetworkProtocolSerializable.hh"

namespace Serializable
{
  UniSoulNetworkProtocolSerializable
  ::UniSoulNetworkProtocolSerializable(const UniSoulPacket& component) :
    ASerializable(component)
  {
  }

  const UniSoulPacket&
  UniSoulNetworkProtocolSerializable::getSerializableComponent() const
  {
    return _component;
  }

  void UniSoulNetworkProtocolSerializable
  ::setSerializableComponent(const UniSoulPacket& component)
  {
    _component = component;
  }
  
  template <class Archive>
  void UniSoulNetworkProtocolSerializable::serialize(Archive& ar,
						     const unsigned int)
  {
    ar & boost::serialization::base_object<ASerializable<UniSoulPacket>>(*this);
    ar & _component.header.header_size;
    ar & _component.header.communication;
    ar & _component.header.request;
    ar & _component.data.data_size;
    ar & _component.data.data;
  }
}
BOOST_CLASS_EXPORT(Serializable::UniSoulNetworkProtocolSerializable)
