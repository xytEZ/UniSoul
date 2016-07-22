#include "RequestEnumConverter.hh"

namespace Network
{
  namespace Protocol
  {
    RequestEnumConverter::RequestEnumConverter() :
      _requests
    {
      { Request::CONNECTION, "Connection" },
	{ Request::DECONNECTION, "Deconnection" }
    }
      {
      }

    std::string RequestEnumConverter
    ::convertRequestToString(Request request) const
    {
      return _requests.at(request);
    }
  }
}
