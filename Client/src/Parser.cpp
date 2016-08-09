#include <cstring>
#include <regex>
#include <stdexcept>

#include "Parser.hh"

const std::map<std::string, std::vector<std::string>>
  Parser::REGEX_COMMANDS =
  {
    { "Help", { } },
    { "Quit", { } },
    { "Connect", { } },
    { "Deconnect", { } },
    { "Message", { "^[a-zA-Z0-9]+$", ".+"} }
  };

std::vector<Parser::ParsedInput>
Parser::getParsedInput(std::sregex_token_iterator& it) const
{
  std::vector<Parser::ParsedInput>	parsedInputArray;

  parsedInputArray.push_back(getParsedInputCommand(*it));
  try
    {
      std::sregex_token_iterator	end;
     
      std::string			cmd(*it++); 
      unsigned int			nbRequiredParam =
	REGEX_COMMANDS.at(cmd).size();
      unsigned int			n = 1;

      while (it != end)
	{
	  parsedInputArray
	    .push_back(getParsedInputParam(cmd, *it++, n - 1));
	  ++n;
	}
      if (n < nbRequiredParam)
	parsedInputArray.push_back({ Parser::ParsedState::MISSING_ARG, "" });
    }
  catch (const std::out_of_range&) { }
  return parsedInputArray;
}

Parser::ParsedInput
Parser::getParsedInputCommand(const std::string& token) const
{
  Parser::ParsedInput	parsedInput;
  
  parsedInput.state =
    (REGEX_COMMANDS.find(token) != REGEX_COMMANDS.end()) ?
    Parser::ParsedState::VALID : Parser::ParsedState::INVALID_CMD;
  parsedInput.what = token;
  return parsedInput;
}

Parser::ParsedInput
Parser::getParsedInputParam(const std::string& cmd,
			    const std::string& token,
			    int n) const
{
  Parser::ParsedInput	parsedInput;

  try
    {
      std::regex	regex(REGEX_COMMANDS.at(cmd).at(n));

      parsedInput.state = std::regex_match(token, regex) ?
	Parser::ParsedState::VALID : Parser::ParsedState::INVALID_ARG;
    }
  catch (const std::out_of_range&)
    {
      parsedInput.state = Parser::ParsedState::EXCESS_ARG;
    }
  parsedInput.what = token;
  return parsedInput;
}
