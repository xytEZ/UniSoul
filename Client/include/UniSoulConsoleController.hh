#ifndef UNI_SOUL_CONSOLE_CONTROLLER_HH_
# define UNI_SOUL_CONSOLE_CONTROLLER_HH_

# include "ObserverOutputResult.hh"
# include "AController.hpp"

namespace Controller
{
  class UniSoulConsoleController : public AController<Observer::OutputResult>
  {
  public :
    UniSoulConsoleController(const ModelPtr&);
    virtual ~UniSoulConsoleController() = default;
    virtual void performAction(const std::string&);
  };
}

#endif /* !UNI_SOUL_CONSOLE_CONTROLLER_HH_ */
