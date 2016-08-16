#include <utility>

#include "UnknownCommandException.hh"

namespace Exception
{
  namespace Command
  {
    UnknownCommand::UnknownCommand(const std::string& what_arg) :
      std::runtime_error(std::move(what_arg))
    {
    }
  }
}
