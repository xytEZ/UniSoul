#ifndef UDP_SOCKET_CLIENT_LINUX_HPP_
# define UDP_SOCKET_CLIENT_LINUX_HPP_

# include <cstddef>
# include <array>

# include "IUDPSocketClient.hpp"

namespace Network
{
  template <std::size_t N = 256>
  class UDPSocketClientLinux : public IUDPSocketClient<int>
  {
  private :
    int			_fd;
    std::array<char, N>	_buffer;
    
  public :
    UDPSocketClientLinux();
    virtual ~UDPSocketClientLinux() = default;
    virtual bool open();
    virtual bool close();
    virtual const int& getDescriptor() const;
    virtual bool sendTo(const std::string&, const struct sockaddr *);
    virtual std::string recvFrom(struct sockaddr *);
  };

  template <std::size_t N>
  UDPSocketClientLinux<N>::UDPSocketClientLinux() : _fd(0) { }

  template <std::size_t N>
  bool UDPSocketClientLinux<N>::open() { return true; }
  
  template <std::size_t N>
  bool UDPSocketClientLinux<N>::close() { return true; }
  
  template <std::size_t N>
  const int& UDPSocketClientLinux<N>::getDescriptor() const { return _fd; }
  
  template <std::size_t N>
  bool UDPSocketClientLinux<N>::sendTo(const std::string&,
				       const struct sockaddr *)
  {
    return true;
  }

  template <std::size_t N>
  std::string UDPSocketClientLinux<N>::recvFrom(struct sockaddr *)
  {
    return "";
  }
}

#endif /* !UDP_SOCKET_CLIENT_LINUX_HPP_ */
