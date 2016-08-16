#ifndef SERIALIZATION_FAIL_EXCEPTION_HH_
# define SERIALIZATION_FAIL_EXCEPTION_HH_

# include <string>
# include <stdexcept>

namespace Exception
{
  namespace Serialization
  {
    class SerializationFail : public std::runtime_error
    {
    public :
      SerializationFail(const std::string&);
    };
  }
}

#endif /* !SERIALIZATION_FAIL_EXCEPTION_HH_ */
