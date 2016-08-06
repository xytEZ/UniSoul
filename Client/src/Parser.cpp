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
    { "Deconnect", { } }
  };

std::vector<Parser::ParsedInput>
Parser::getParsedInput(const std::string& input) const
{
  const constexpr char			DELIMETERS[] = " \t";
  std::vector<Parser::ParsedInput>	parsedInputArray;
  char					*inputCopy;
  char					*tmp;
  int					inputLength;

  inputLength = input.length();
  inputCopy = new char[inputLength + 1];
  std::strncpy(inputCopy, input.c_str(), inputLength);
  inputCopy[inputLength] = '\0';
  if ((tmp = std::strtok(inputCopy, DELIMETERS)))
    {
      std::string	cmd(tmp);
      
      parsedInputArray.push_back(getParsedInputCommand(cmd));
      for (int n = 0; (tmp = std::strtok(nullptr, DELIMETERS)); ++n)
	parsedInputArray.push_back(getParsedInputParam(cmd, tmp, n));
    }
  delete[] inputCopy;
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
