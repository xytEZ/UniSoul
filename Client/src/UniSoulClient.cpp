#include "UniSoulClient.hh"

namespace Model
{
  UniSoulClient::UniSoulClient(const std::string&,
			       unsigned short)
  {
    _init();
  }

  UniSoulClient::~UniSoulClient()
  {
    _close();
  }

  void UniSoulClient::execute(const std::string&)
  {
  }

  inline void UniSoulClient::_init()
  {
  }

  inline void UniSoulClient::_close()
  {
  }
}
