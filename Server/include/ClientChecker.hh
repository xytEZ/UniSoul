#ifndef CLIENT_CHECKER_HH_
# define CLIENT_CHECKER_HH_

# include <memory>
# include <string>

# include "IPersistentDataInteractor.hpp"

namespace Persistence
{
  class ClientChecker
  {
  private :
    using PersistentDataInteractorPtr =
      std::shared_ptr<Persistence::IPersistentDataInteractor<bool>>;
    
  private :
    PersistentDataInteractorPtr	_persistentDataInteractorPtr;
    
  public :
    ClientChecker(const PersistentDataInteractorPtr&);
    ~ClientChecker() = default;
    void setPersistentDataInteractorPtr(const PersistentDataInteractorPtr&);
    bool check(const std::string&) const;
  };
}

#endif /* !CLIENT_CHECKER_HH_ */
