#include <map>
#include <functional>
#include <algorithm>

#include "UniSoulConsoleController.hh"

namespace Controller
{ 
  UniSoulConsoleController
  ::UniSoulConsoleController(const ModelPtr& modelPtr) : AController(modelPtr)
  {
  }
  
  void UniSoulConsoleController::performAction(const std::string& input)
  {
    static const std::map<Parser::ParsedState, std::function<const char *()>>
      FUNCS_ERR_MSG =
      {
	{ Parser::ParsedState::INVALID_CMD,
	  []() -> const char * { return "Command is invalid."; } },
	{ Parser::ParsedState::INVALID_ARG,
	  []() -> const char * { return "Argument(s) is invalid."; } },
	{ Parser::ParsedState::MISSING_ARG,
	  []() -> const char * { return "Argument(s) is missing."; } },
	{ Parser::ParsedState::EXCESS_ARG,
	  []() -> const char * { return "Excess argument."; } }
      };
    
    if (!input.empty()
	&& input.find_first_not_of(Parser::DELIMETERS) != std::string::npos)
      {
	std::vector<Parser::ParsedInput>			parsedInputArray;
	std::vector<Parser::ParsedInput>::const_iterator	constIt;
	std::string						errMsg;
	
	parsedInputArray = _parser.getParsedInput(input);
	constIt = std::find_if
	  (parsedInputArray.cbegin(),
	   parsedInputArray.cend(),
	   [&errMsg](const Parser::ParsedInput& parsedInput) -> bool
	   {
	     if (FUNCS_ERR_MSG.find(parsedInput.state) != FUNCS_ERR_MSG.end())
	       {
		 errMsg = FUNCS_ERR_MSG.at(parsedInput.state)();
		 return true;
	       }
	     return false;
	   });
	if (constIt != parsedInputArray.cend())
	  _modelPtr->notifyObservers({ true, errMsg });
	else
	  _modelPtr->execute(parsedInputArray);
      }
  }
}
