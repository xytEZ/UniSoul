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
    static const
      std::map<Parser::ParsedState, std::function<std::string()>>
      FUNCS_ERR_MSG =
      {
	{ Parser::ParsedState::INVALID_CMD,
	  []() -> std::string { return "Command is invalid."; } },
	{ Parser::ParsedState::INVALID_ARG,
	  []() -> std::string { return "Argument(s) is invalid."; } },
	{ Parser::ParsedState::EXCESS_ARG,
	  []() -> std::string { return "Excess argument."; } }
      };
    
    if (!input.empty())
      {
	std::vector<Parser::ParsedInput>	parsedInputArray;
	
	std::vector<Parser::ParsedInput>
	  ::const_iterator			it;
	
	std::string				errMsg;
	
	parsedInputArray = _parser.getParsedInput(input);
	it = std::find_if
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
	if (it != parsedInputArray.end())
	  _modelPtr->notifyObservers({ true, errMsg });
	else
	  _modelPtr->execute(parsedInputArray);
      }
  }
}
