#include "BoostServiceWrapper.hh"

namespace Wrapper
{
  boost::asio::io_service	BoostServiceWrapper::_ios;
  
  BoostServiceWrapper::BoostServiceWrapper() { }
  BoostServiceWrapper::~BoostServiceWrapper() { }
  boost::asio::io_service& BoostServiceWrapper::getContent() { return _ios; }
}
