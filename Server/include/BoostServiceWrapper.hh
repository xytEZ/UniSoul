#ifndef BOOST_SERVICE_WRAPPER_HH_
# define BOOST_SERVICE_WRAPPER_HH_

# include <boost/asio.hpp>
# include "IWrapper.hpp"

namespace Wrapper
{
  class BoostServiceWrapper : public IWrapper<boost::asio::io_service>
  {
  private :
    static boost::asio::io_service	_ios;
    
  public :
    BoostServiceWrapper() = default;
    virtual ~BoostServiceWrapper() = default;
    virtual boost::asio::io_service& getContent();
  };
}

#endif /* !BOOST_SERVICE_WRAPPER_HH_ */
