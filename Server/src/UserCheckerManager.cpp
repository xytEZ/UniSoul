#include <sstream>
#include "UserCheckerManager.hh"

namespace Persistence
{
  namespace Manager
  {
    UserCheckerManager
    ::UserCheckerManager(const PersistentDataInteractorPtr& persistentDataInteractorPtr) :
      _persistentDataInteractorPtr(std::move(persistentDataInteractorPtr))
    {
    }

    UserCheckerManager::~UserCheckerManager() { }

    void UserCheckerManager
    ::setPersistentDataInteractorPtr(const PersistentDataInteractorPtr& persistentDataInteractorPtr)
    {
      _persistentDataInteractorPtr = std::move(persistentDataInteractorPtr);
    }

    template <typename... Args>
    bool UserCheckerManager::userChecker(const char separator,
					 const Args&... args) const
    {
      return _persistentDataInteractorPtr
	->find(concatArguments(separator, args...));
    }

    template <typename T, typename... Args>
    std::string UserCheckerManager::concatArguments(const char separator,
						    const T& value,
						    const Args&... args)
    {
      return concatArguments(separator, value);
    }

    template <typename T>
    std::string UserCheckerManager::concatArguments(const char separator,
						    const T& value)
    {
      std::ostringstream        oss;

      oss << value << separator;
      return oss.str();
    }
  }
}
