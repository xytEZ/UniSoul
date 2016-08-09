#ifndef UNI_SOUL_CONSOLE_CONTROLLER_HH_
# define UNI_SOUL_CONSOLE_CONTROLLER_HH_

# include "ObserverOutputResult.hh"
# include "Parser.hh"
# include "AController.hpp"

namespace Controller
{
  class UniSoulConsoleController : public AController<Observer::OutputResult>
  {
  private :
    Parser	_parser;
    
  public :
    UniSoulConsoleController(const ModelPtr&);
    virtual ~UniSoulConsoleController() = default;
    virtual void performAction(const std::string&);

  private :
    bool parsedInputArrayIsCorrect(const std::vector<Parser::ParsedInput>&,
				   std::string&) const;
  };
}

#endif /* !UNI_SOUL_CONSOLE_CONTROLLER_HH_ */
