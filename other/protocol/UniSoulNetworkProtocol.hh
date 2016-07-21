#ifndef UNI_SOUL_NETWORK_PROTOCOL_HH_
# define UNI_SOUL_NETWORK_PROTOCOL_HH_

typedef unsigned char		*byte_array;

namespace Network
{
  typedef enum	e_communication
    {
      TCP = 0x0,
      UDP = 0x1,
      FTP = 0x2
    }		t_communication;

  typedef enum	e_request
    {
      ERROR = 0x0,
      CONNECTION = 0x1,
      DECONNECTION = 0x2
    }		t_request;
  
  namespace Protocol
  {    
    typedef struct		s_header
    {
      unsigned short int	header_size;
      t_communication		communication;
      t_request			request;
    }				t_header;

    typedef struct		s_data
    {
      unsigned short int	data_size;
      byte_array		data;
    }				t_data;

    typedef struct		s_uni_soul_packet
    {
      t_header			header;
      t_data			data;
    }				t_uni_soul_packet;
  }
}

using UniSoulPacket = Network::Protocol::t_uni_soul_packet;

#endif /* !UNI_SOUL_NETWORK_PROTOCOL_HH_ */
