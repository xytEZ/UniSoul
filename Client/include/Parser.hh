#ifndef PARSER_HH_
# define PARSER_HH_

# include <vector>
# include <map>
# include <string>

class Parser
{
public :
  enum class ParsedState
  {
    VALID,
      INVALID_CMD,
      INVALID_ARG,
      MISSING_ARG,
      EXCESS_ARG,
      };
  
  struct ParsedInput
  {
    ParsedState     state;
    std::string     what;
  };

public :
  static constexpr const char	*DELIMETERS = " \t";
  
private :
  static const std::map<std::string,
			std::vector<std::string>>	REGEX_COMMANDS;
  
public :
  Parser() = default;
  ~Parser() = default;
  std::vector<ParsedInput> getParsedInput(const std::string&) const;
  
private :
  ParsedInput getParsedInputCommand(const std::string&) const;
  
  ParsedInput getParsedInputParam(const std::string&,
				  const std::string&,
				  int) const;
};

#endif /* !PARSER_HH_ */
