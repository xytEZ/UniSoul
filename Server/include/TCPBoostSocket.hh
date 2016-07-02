#ifndef TCP_BOOST_SOCKET_HH_
# define TCP_BOOST_SOCKET_HH_

# include <boost/asio.hpp>
# include "ITCPSocket.hh"

namespace Network
{
  class TCPBoostSocket : virtual public ITCPSocket
  {
  private :
    boost::asio::ip::tcp::socket	_socket;
    
  public :
    TCPBoostSocket(boost::asio::io_service&);
    virtual ~TCPBoostSocket();
    virtual bool open(int, int, int);
    virtual bool close();
    virtual bool send(const std::string&) const;
    virtual std::string recv() const;
  };
}

#endif /* !TCP_BOOST_SOCKET_HH_ */
