#ifndef CLIENT_INFO_HH_
# define CLIENT_INFO_HH_

# include <string>

namespace Info
{
  struct ClientInfo
  {
    std::string	firstName;
    std::string	lastName;
    std::string	email;
    std::string	address;
    std::string	port;
  };
}

#endif /* !CLIENT_INFO_HH_ */
