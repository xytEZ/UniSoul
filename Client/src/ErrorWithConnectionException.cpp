#include <utility>

#include "ErrorWithConnectionException.hh"

namespace Exception
{
  namespace Network
  {
    ErrorWithConnection::ErrorWithConnection(const std::string& what_arg) :
      std::runtime_error(std::move(what_arg))
    {
    }
  }
}
