#include <utility>
#include <cstring>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/binary_object.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include "UniSoulNetworkProtocolSerializable.hh"

namespace Serializable
{
  UniSoulNetworkProtocolSerializable
  ::UniSoulNetworkProtocolSerializable(const Network::Protocol
				       ::UniSoulPacket& component) :
    ASerializable<Network::Protocol::UniSoulPacket>(component)
  {
  }

  UniSoulNetworkProtocolSerializable::~UniSoulNetworkProtocolSerializable()
  {
    delete _component.data;
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

  template <>
  void UniSoulNetworkProtocolSerializable::serialize
  <boost::archive::binary_oarchive>(boost::archive::binary_oarchive& ar,
				    const unsigned int)
  {
    ar & boost::serialization::base_object
      <ASerializable<Network::Protocol::UniSoulPacket>>(*this)
      & _component.communication
      & _component.command
      & boost::serialization::make_binary_object(_component.data,
						 std::strlen(_component.data));
  }
}
