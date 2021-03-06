#include <map>
#include <functional>
#include <algorithm>
#include <iostream>
#include <stdexcept>

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
      {
	std::regex				regex("[\t ]+");
	
	std::sregex_token_iterator		it(input.begin(),
						   input.end(),
						   regex,
						   -1);

	std::vector<Parser::ParsedInput>	parsedInputArray;
	std::string				errMsg;
	
	parsedInputArray = _parser.getParsedInput(it);
	if (parsedInputArrayIsCorrect(parsedInputArray, errMsg))
	  _modelPtr->execute(parsedInputArray);
	else
	  _modelPtr->notifyObservers({ true, errMsg });
      }
  }

  bool
  UniSoulConsoleController
  ::parsedInputArrayIsCorrect(const std::vector<Parser::ParsedInput>&
			      parsedInputArray,
			      std::string& errMsg) const
  {
    static const std::map<Parser::ParsedState, std::function<const char *()>>
      FUNCS_ERR_MSG =
      {
	{ Parser::ParsedState::INVALID_CMD,
	  []() -> const char * { return "Command is invalid\n"; } },
	{ Parser::ParsedState::INVALID_ARG,
	  []() -> const char * { return "Argument(s) is invalid\n"; } },
	{ Parser::ParsedState::MISSING_ARG,
	  []() -> const char * { return "Argument(s) is missing\n"; } },
	{ Parser::ParsedState::EXCESS_ARG,
	  []() -> const char * { return "Excess argument\n"; } }
      };
    
    return std::find_if
      (parsedInputArray.cbegin(),
       parsedInputArray.cend(),
       [&errMsg](const Parser::ParsedInput& parsedInput) -> bool
       {
	 try
	   {
	     errMsg = FUNCS_ERR_MSG.at(parsedInput.state)();
	   }
	 catch (const std::out_of_range&) { return false; }
	 return true;
       }) == parsedInputArray.cend();
  }
}
