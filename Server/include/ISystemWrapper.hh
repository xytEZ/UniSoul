#ifndef ISYSTEM_WRAPPER_HH_
# define ISYSTEM_WRAPPER_HH_

# include <map>
# include <string>
# include <boost/any.hpp>
# include "IWrapper.hpp"

namespace Wrapper
{
  namespace System
  {
    using VariantMap = std::map<std::string, boost::any>;
    
    class ISystemWrapper : public IWrapper<VariantMap>
    {
    public :
      virtual ~ISystemWrapper() { }
      virtual VariantMap& getContent() = 0;
    };
  }
}

#endif /* !ISYSTEM_WRAPPER_HH_ */
