#include <string>
#include <cstring>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "UniSoulNetworkProtocol.hh"

namespace Network
{
  namespace Protocol
  {
    template <>
    void UniSoulPacket::serialize
    <boost::archive::binary_oarchive>(boost::archive::binary_oarchive& ar,
				      const unsigned int)
    {
      std::string	s(data);
      
      ar & communication & command & s;
    }

    template <>
    void UniSoulPacket::serialize
    <boost::archive::binary_iarchive>(boost::archive::binary_iarchive& ar,
				      const unsigned int)
    {
      std::string	s;
      std::size_t	size;
      
      ar & communication & command & s;
      size = s.size();
      data = new char[size + 1];
      std::strcpy(data, s.c_str());
      data[size] = '\0';
    }
  }
}
