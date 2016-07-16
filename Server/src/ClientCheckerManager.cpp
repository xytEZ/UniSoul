#include <sstream>
#include "ClientCheckerManager.hh"

namespace Persistence
{
  namespace Manager
  {
    ClientCheckerManager
    ::ClientCheckerManager(const PersistentDataInteractorPtr& persistentDataInteractorPtr) :
      _persistentDataInteractorPtr(std::move(persistentDataInteractorPtr))
    {
    }

    ClientCheckerManager::~ClientCheckerManager() { }

    void ClientCheckerManager
    ::setPersistentDataInteractorPtr(const PersistentDataInteractorPtr& persistentDataInteractorPtr)
    {
      _persistentDataInteractorPtr = std::move(persistentDataInteractorPtr);
    }

    template <typename... Args>
    bool ClientCheckerManager::clientChecker(const char separator,
					     const Args&... args) const
    {
      return _persistentDataInteractorPtr
	->find(concatArguments(separator, args...));
    }

    template <typename T, typename... Args>
    std::string ClientCheckerManager::concatArguments(const char separator,
						      const T& value,
						      const Args&... args)
    {
      return concatArguments(separator, value);
    }

    template <typename T>
    std::string ClientCheckerManager::concatArguments(const char separator,
						      const T& value)
    {
      std::ostringstream        oss;

      oss << value << separator;
      return oss.str();
    }
  }
}
