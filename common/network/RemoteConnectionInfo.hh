#ifndef REMOTE_CONNECTION_INFO_HH_
# define REMOTE_CONNECTION_INFO_HH_

# include <string>

namespace Network
{
  struct RemoteConnectionInfo
  {
    std::string		login;
    std::string		listeningAddress;
    unsigned short	listeningPort;

    RemoteConnectionInfo(const std::string& = "",
			 const std::string& = "",
			 unsigned short = 0);
  };
}

#endif /* !REMOTE_CONNECTION_INFO_HH_ */
