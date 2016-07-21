#ifndef IPACKET_STATE_CHECKER_HPP_
# define IPACKET_STATE_CHECKER_HPP_

namespace Network
{
  template <typename T>
  class IPacketStateChecker
  {
  public :
    virtual ~IPacketStateChecker() = default;
    virtual void setPacket(const T&) = 0;
    virtual bool checkPacket() const = 0;
  };
}

#endif /* !IPACKET_STATE_CHECKER_HPP_ */
