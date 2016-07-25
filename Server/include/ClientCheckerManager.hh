#ifndef CLIENT_CHECKER_MANAGER_HH_
# define CLIENT_CHECKER_MANAGER_HH_

# include <memory>
# include <string>
# include "IPersistentDataInteractor.hpp"

namespace Persistence
{
  namespace Manager
  {
    class ClientCheckerManager
    {
    private :
      using PersistentDataInteractorPtr =
	std::shared_ptr<Persistence::IPersistentDataInteractor<bool>>;

    private :
      PersistentDataInteractorPtr	_persistentDataInteractorPtr;

    public :
      ClientCheckerManager(const PersistentDataInteractorPtr&);
      ~ClientCheckerManager() = default;
      void setPersistentDataInteractorPtr(const PersistentDataInteractorPtr&);
      bool checkClient(const std::string&) const;
    };
  } 
}

#endif /* !CLIENT_CHECKER_MANAGER_HH_ */
