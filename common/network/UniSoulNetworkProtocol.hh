#ifndef UNI_SOUL_NETWORK_PROTOCOL_HH_
# define UNI_SOUL_NETWORK_PROTOCOL_HH_

# include <boost/serialization/serialization.hpp>

namespace Network
{ 
  namespace Protocol
  {
    enum Communication
      {
	TCP = 0x0,
	UDP = 0x1,
	FTP = 0x2
      };

    struct UniSoulPacket
    {
      Communication		communication;
      unsigned short int	command;
      char			*data;
      
    private :
      friend class boost::serialization::access;

    private :
      template <class Archive>
      void serialize(Archive&, const unsigned int);
    };
  }
}

#endif /* !UNI_SOUL_NETWORK_PROTOCOL_HH_ */
