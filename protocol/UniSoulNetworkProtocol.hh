#ifndef UNI_SOUL_NETWORK_PROTOCOL_HH_
# define UNI_SOUL_NETWORK_PROTOCOL_HH_

# define TEXT_EXCHANGE		0x0
# define AUDIO_EXCHANGE		0x1
# define FILE_TRANFERT_EXCHANGE	0x2

# define CONNECTION_REQ		0
# define DECONNECTION_REQ	1

typedef unsigned char		byte;
typedef unsigned float		u_float;
typedef unsigned short int	u_short_int;

namespace Network
{
  namespace Protocol
  {
    struct Header
    {
      byte		communication_type;
      u_float		request_type;
      Identifier	identifier;
    };

    struct Identifier
    {
      u_short_int	first_name_size;
      byte		*first_name;
      u_short_int	last_name_size;
      byte		*last_name;
      u_short_int	email_size;
      byte		*email;
    };
    
    struct Data
    {
      u_short_int	data_size;
      byte		*data;
    };
  }
}

#endif /* !UNI_SOUL_NETWORK_PROTOCOL_HH_ */
