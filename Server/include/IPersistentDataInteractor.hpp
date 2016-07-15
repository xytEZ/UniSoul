#ifndef IPERSISTENT_DATA_INTERACTOR_HPP_
# define IPERSISTENT_DATA_INTERACTOR_HPP_

# include <string>

namespace Persistence
{
  template <typename T>
  class IPersistentDataInteractor
  {
  public :
    virtual ~IPersistentDataInteractor() { };
    virtual void init() = 0;
    virtual T find(const std::string&) const = 0;
    virtual void close() = 0;
  };
}

#endif /* !IPERSISTENT_DATA_INTERACTOR_HPP_ */
