#ifndef BOOST_SERVICE_WRAPPER_HH_
# define BOOST_SERVICE_WRAPPER_HH_

# include <boost/asio.hpp>
# include "ILibraryServiceWrapper.hpp"

namespace Wrapper
{
  class BoostServiceWrapper : public ILibraryServiceWrapper<boost::asio::io_service>
  {
  private :
    static boost::asio::io_service	_ios;
    
  public :
    BoostServiceWrapper();
    virtual ~BoostServiceWrapper();
    virtual boost::asio::io_service& getContent();
  };
}

#endif /* !BOOST_SERVICE_WRAPPER_HH_ */
