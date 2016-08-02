#include "UniSoulConsoleController.hh"

namespace Controller
{
  UniSoulConsoleController
  ::UniSoulConsoleController(const ModelPtr& modelPtr) : AController(modelPtr)
  {
  }

  void UniSoulConsoleController::performAction(const std::string& input)
  {
    if (!input.empty())
      _modelPtr->execute(input);
  }
}
