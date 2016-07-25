#include <utility>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/export.hpp>
#include "UniSoulNetworkProtocolSerializable.hh"

namespace Serializable
{
  UniSoulNetworkProtocolSerializable
  ::UniSoulNetworkProtocolSerializable(const Network::Protocol
				       ::UniSoulPacket& component) :
    ASerializable(component)
  {
  }

  const Network::Protocol::UniSoulPacket& UniSoulNetworkProtocolSerializable
  ::getSerializableComponent() const
  {
    return _component;
  }

  void UniSoulNetworkProtocolSerializable
  ::setSerializableComponent(const Network::Protocol::UniSoulPacket& component)
  {
    _component = component;
  }
  
  template <class Archive>
  void UniSoulNetworkProtocolSerializable::serialize(Archive& ar,
						     const unsigned int)
  {
    ar & boost::serialization::base_object
      <ASerializable<Network::Protocol::UniSoulPacket>>(*this);
    ar & _component.header.header_size;
    ar & _component.header.communication;
    ar & _component.header.command;
    ar & _component.data.data_size;
    ar & _component.data.data;
  }
}
BOOST_CLASS_EXPORT(Serializable::UniSoulNetworkProtocolSerializable)
