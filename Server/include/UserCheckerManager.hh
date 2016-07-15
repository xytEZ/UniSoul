#ifndef USER_CHECKER_MANAGER_HH_
# define USER_CHECKER_MANAGER_HH_

# include <memory>
# include <string>
# include "IPersistentDataInteractor.hpp"

namespace Persistence
{
  namespace Manager
  {
    class UserCheckerManager
    {
    private :
      using PersistentDataInteractorPtr =
	std::shared_ptr<Persistence::IPersistentDataInteractor<bool>>;

    private :
      PersistentDataInteractorPtr	_persistentDataInteractorPtr;

    public :
      UserCheckerManager(const PersistentDataInteractorPtr&);
      ~UserCheckerManager();
      void setPersistentDataInteractorPtr(const PersistentDataInteractorPtr&);
    
      template <typename... Args>
      bool userChecker(const char, const Args&...) const;

    private :
      template <typename T, typename... Args>
      static std::string concatArguments(const char, const T&, const Args&...);

      template <typename T>
      static std::string concatArguments(const char, const T&);
    };
  } 
}

#endif /* !USER_CHECKER_MANAGER_HH_ */
