#ifndef IOBSERVER_HPP_
# define IOBSERVER_HPP_

namespace Observer
{
  template <typename T>
  class IObserver
  {
  public :
    virtual ~IObserver() = default;
    virtual void update(const T&) = 0;
  };
}

#endif /* !IOBSERVER_HPP_ */
