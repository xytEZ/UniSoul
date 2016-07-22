#ifndef REQUEST_ENUM_CONVERTER_HH_
# define REQUEST_ENUM_CONVERTER_HH_

# include <map>
# include <string>
# include "UniSoulNetworkProtocol.hh"

namespace Network
{
  namespace Protocol
  {
    class RequestEnumConverter
    {
    private :
      std::map<Request, std::string>	_requests;

    public :
      RequestEnumConverter();
      ~RequestEnumConverter() = default;
      std::string convertRequestToString(Request) const;
    };
  }
}

#endif /* !REQUEST_ENUM_CONVERTER_HH_ */
