#ifndef TCP_SOCKET_CLIENT_LINUX_HPP_
# define TCP_SOCKET_CLIENT_LINUX_HPP_

# include <system_error>
# include <array>
# include <string>
# include <cstddef>
# include <cstring>

# include <cerrno>
# include <unistd.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <netinet/in.h>

# include "ITCPSocketClient.hpp"

namespace Network
{
  template <std::size_t N>
  class TCPSocketClientLinux : public ITCPSocketClient<int>
  {
  private :
    int			_fd;
    struct sockaddr_in	_clientAddr;
    struct sockaddr_in	_serverAddr;
    unsigned short	_port;
    std::array<char, N>	_buffer;
    
  public :
    TCPSocketClientLinux(const std::string&, unsigned short);
    virtual ~TCPSocketClientLinux() = default;
    virtual bool open();
    virtual bool close();
    virtual const int& getDescriptor() const;
    virtual bool send(const std::string&);
    virtual std::string recv();
    virtual std::string getAddress() const;
    virtual unsigned short getPort() const;
    virtual bool connect();
  };

  template <std::size_t N>
  TCPSocketClientLinux<N>::TCPSocketClientLinux(const std::string& hostname,
						unsigned short port)
  {
    _serverAddr.sin_addr.s_addr = ::inet_addr(hostname.c_str());
    _serverAddr.sin_family = AF_INET;
    _serverAddr.sin_port = ::htons(port);
  }
  
  template <std::size_t N>
  bool TCPSocketClientLinux<N>::open()
  {
    if ((_fd = ::socket(AF_INET, SOCK_STREAM, 0)) == -1)
      throw std::system_error(errno, std::system_category());
    return true;
  }
  
  template <std::size_t N>
  bool TCPSocketClientLinux<N>::close()
  {
    return !::close(_fd);
  }
  
  template <std::size_t N>
  const int& TCPSocketClientLinux<N>::getDescriptor() const { return _fd; }
  
  template <std::size_t N>
  bool TCPSocketClientLinux<N>::send(const std::string& msg)
  {
    if (::send(_fd,
	       msg.c_str(),
	       N,
	       MSG_DONTWAIT | MSG_NOSIGNAL | MSG_MORE) == -1)
      throw std::system_error(errno, std::system_category());
    return true;
  }
  
  template <std::size_t N>
  std::string TCPSocketClientLinux<N>::recv()
  {
    _buffer.fill(0);
    if (::recv(_fd,
	       _buffer.data(),
	       N,
	       MSG_DONTWAIT | MSG_PEEK | MSG_TRUNC) == -1)
      throw std::system_error(errno, std::system_category());
    return std::string(_buffer.cbegin(), _buffer.cend());
  }

  template <std::size_t N>
  std::string TCPSocketClientLinux<N>::getAddress() const
  {
    int	addrLen = sizeof(_clientAddr);

    if (::getsockname(_fd,
		      (struct sockaddr *)&_clientAddr,
		      reinterpret_cast<socklen_t *>(&addrLen)) == -1)
      throw std::system_error(errno, std::system_category());
    return ::inet_ntoa(_clientAddr.sin_addr);
  }
  
  template <std::size_t N>
  unsigned short TCPSocketClientLinux<N>::getPort() const
  {
    int	addrLen = sizeof(_clientAddr);

    if (::getsockname(_fd,
		      (struct sockaddr *)&_clientAddr,
		      reinterpret_cast<socklen_t *>(&addrLen)) == -1)
      throw std::system_error(errno, std::system_category());
    return (::ntohs(_clientAddr.sin_port));
  }

  template <std::size_t N>
  bool TCPSocketClientLinux<N>::connect()
  {
    if (::connect(_fd,
		  (struct sockaddr *)&_serverAddr,
		  sizeof(_serverAddr)) == -1)
      throw std::system_error(errno, std::system_category());
    return true;
  }
}

#endif /* !TCP_SOCKET_CLIENT_LINUX_HPP_ */
