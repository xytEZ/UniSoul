#ifndef COMPLEMENTARY_FUNCTION_HPP_
# define COMPLEMENTARY_FUNCTION_HPP_

# include <sstream>

namespace Tool
{
  template <typename T>
  T convert_string_to(const std::string& s)
  {
    std::istringstream	ss(s);
    T			value;

    ss >> value;
    return value;
  }
}

#endif /*! COMPLEMENTARY_FUNCTION_HPP_ */
