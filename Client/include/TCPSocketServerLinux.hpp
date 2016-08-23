#ifndef TCP_SOCKET_SERVER_LINUX_HPP_
# define TCP_SOCKET_SERVER_LINUX_HPP_

# include <system_error>
# include <array>
# include <string>
# include <cstddef>
# include <cstring>
# include <memory>

# include <cerrno>
# include <unistd.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <netinet/in.h>

# include "RemoteConnectionInfo.hh"
# include "TCPSocketClientLinux.hpp"
# include "ITCPSocketServer.hpp"

namespace Network
{
  template <std::size_t MAX_SOCKET = 15, std::size_t N = 256>
  class TCPSocketServerLinux :
    public ITCPSocketServer<int, std::shared_ptr<Network::ITCPSocket<int>>>
  {
  private :
    int				_fd;
    struct sockaddr_in		_addr;
    std::array<char, N>		_buffer;
    RemoteConnectionInfo	_remoteConnectionInfo;
    
  public :    
    TCPSocketServerLinux(const std::string&,
			 unsigned short,
			 const RemoteConnectionInfo& = { "", "", 0 } );
    
    virtual ~TCPSocketServerLinux() = default;
    virtual bool open();
    virtual bool close();
    virtual const int& getDescriptor() const;
    virtual bool send(const std::string&);
    virtual std::string recv();
    virtual std::string getAddress() const;
    virtual unsigned short getPort() const;
    virtual const RemoteConnectionInfo& getRemoteConnectionInfo() const;
    virtual void setRemoteConnectionInfo(const RemoteConnectionInfo&);
    virtual bool bind();
    virtual bool listen();
    virtual std::shared_ptr<Network::ITCPSocket<int>> accept();
  };
  
  template <std::size_t MAX_SOCKET, std::size_t N>
  TCPSocketServerLinux<MAX_SOCKET, N>
  ::TCPSocketServerLinux(const std::string& hostname,
			 unsigned short port,
			 const RemoteConnectionInfo& remoteConnectionInfo) :
    _remoteConnectionInfo(remoteConnectionInfo)
  {
    _addr.sin_addr.s_addr = ::inet_addr(hostname.c_str());
    _addr.sin_family = AF_INET;
    _addr.sin_port = ::htons(port);
  }

  template <std::size_t MAX_SOCKET, std::size_t N>
  bool TCPSocketServerLinux<MAX_SOCKET, N>::open()
  {
    if ((_fd = ::socket(AF_INET, SOCK_STREAM, 0)) == -1)
      throw std::system_error(errno, std::system_category());
    return true;
  }

  template <std::size_t MAX_SOCKET, std::size_t N>
  bool TCPSocketServerLinux<MAX_SOCKET, N>::close()
  {
    if (::shutdown(_fd, SHUT_RDWR) == -1)
      throw std::system_error(errno, std::system_category());
    return true;
  }

  template <std::size_t MAX_SOCKET, std::size_t N>
  const int&
  TCPSocketServerLinux<MAX_SOCKET, N>::getDescriptor() const { return _fd; }
  
  template <std::size_t MAX_SOCKET, std::size_t N>
  bool TCPSocketServerLinux<MAX_SOCKET, N>::send(const std::string& msg)
  {
    if (::send(_fd,
	       msg.c_str(),
	       N,
	       MSG_DONTWAIT | MSG_NOSIGNAL | MSG_MORE) == -1)
      throw std::system_error(errno, std::system_category());
    return true;
  }

  template <std::size_t MAX_SOCKET, std::size_t N>
  std::string TCPSocketServerLinux<MAX_SOCKET, N>::recv()
  {
    ssize_t     bytesTransferred;

    if ((bytesTransferred = ::recv(_fd,
				   _buffer.data(),
				   N,
				   MSG_DONTWAIT)) == -1)
      throw std::system_error(errno, std::system_category());
    return std::string(_buffer.data(), bytesTransferred);
  }

  template <std::size_t MAX_SOCKET, std::size_t N>
  std::string TCPSocketServerLinux<MAX_SOCKET, N>::getAddress() const
  {
    return ::inet_ntoa(_addr.sin_addr);
  }

  template <std::size_t MAX_SOCKET, std::size_t N>
  unsigned short TCPSocketServerLinux<MAX_SOCKET, N>::getPort() const
  {
    return ::ntohs(_addr.sin_port);
  }

  template <std::size_t MAX_SOCKET, std::size_t N>
  const RemoteConnectionInfo&
  TCPSocketServerLinux<MAX_SOCKET, N>::getRemoteConnectionInfo() const
  {
    return _remoteConnectionInfo;
  }

  template <std::size_t MAX_SOCKET, std::size_t N>
  void TCPSocketServerLinux<MAX_SOCKET, N>
  ::setRemoteConnectionInfo(const RemoteConnectionInfo& remoteConnectionInfo)
  {
    _remoteConnectionInfo = remoteConnectionInfo;
  }

  template <std::size_t MAX_SOCKET, std::size_t N>
  bool TCPSocketServerLinux<MAX_SOCKET, N>::bind()
  {
    if (::bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1)
      throw std::system_error(errno, std::system_category());
    return true;
  }
  
  template <std::size_t MAX_SOCKET, std::size_t N>
  bool TCPSocketServerLinux<MAX_SOCKET, N>::listen()
  {
    if (::listen(_fd, MAX_SOCKET) == -1)
      throw std::system_error(errno, std::system_category());
    return true;
  }

  template <std::size_t MAX_SOCKET, std::size_t N>
  std::shared_ptr<Network::ITCPSocket<int>>
  TCPSocketServerLinux<MAX_SOCKET, N>::accept()
  {
    int			fd;
    struct sockaddr_in	addr;
    socklen_t		addr_size = sizeof(addr);

    if ((fd = ::accept(_fd, (struct sockaddr *)&addr, &addr_size)) == -1)
      throw std::system_error(errno, std::system_category());
    return std::make_shared<Network::TCPSocketClientLinux<>>(fd, addr);
  }
}

#endif /* !TCP_SOCKET_SERVER_LINUX_HPP_ */
