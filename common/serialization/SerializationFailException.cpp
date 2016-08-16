#include <utility>

#include "SerializationFailException.hh"

namespace Exception
{
  namespace Serialization
  {
    SerializationFail::SerializationFail(const std::string& what_arg) :
      std::runtime_error(std::move(what_arg))
    {
    }
  }
}
