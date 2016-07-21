#include <utility>
#include "CommandNotFoundException.hh"

namespace Exception
{
  namespace Command
  {
    CommandNotFoundException
    ::CommandNotFoundException(const std::string& what_arg) :
      std::runtime_error(std::move(what_arg))
    {
    }
  }
}
