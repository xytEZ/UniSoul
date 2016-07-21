#include "BoostServiceWrapper.hh"

namespace Wrapper
{
  boost::asio::io_service	BoostServiceWrapper::_ios;
  
  boost::asio::io_service& BoostServiceWrapper::getContent() { return _ios; }
}
