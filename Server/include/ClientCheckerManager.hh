#ifndef CLIENT_CHECKER_MANAGER_HH_
# define CLIENT_CHECKER_MANAGER_HH_

# include <memory>
# include <string>
# include <sstream>
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
    
      template <typename... Args>
      bool clientChecker(const char, const Args&...) const;

    private :
      template <typename T, typename... Args>
      static std::string concatArguments(const char, const T&, const Args&...);

      template <typename T>
      static std::string concatArguments(const char, const T&);
    };
  } 
}

#endif /* !CLIENT_CHECKER_MANAGER_HH_ */
