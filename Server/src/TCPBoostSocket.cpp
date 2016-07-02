#include "TCPBoostSocket.hh"

namespace Network
{
  TCPBoostSocket::TCPBoostSocket(boost::asio::io_service& ios) : _socket(ios)
  {
  }
  
  TCPBoostSocket::~TCPBoostSocket() { }
  bool TCPBoostSocket::open(int, int, int) { return true; }
  bool TCPBoostSocket::close() { return true; }
  bool TCPBoostSocket::send(const std::string&) const { return true; }
  std::string TCPBoostSocket::recv() const { return ""; }
}
