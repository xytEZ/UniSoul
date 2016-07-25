#include "ClientCheckerManager.hh"

namespace Persistence
{
  namespace Manager
  {
    ClientCheckerManager
    ::ClientCheckerManager(const PersistentDataInteractorPtr&
			   persistentDataInteractorPtr) :
      _persistentDataInteractorPtr(std::move(persistentDataInteractorPtr))
    {
    }
    
    void ClientCheckerManager
    ::setPersistentDataInteractorPtr(const PersistentDataInteractorPtr&
				     persistentDataInteractorPtr)
    {
      _persistentDataInteractorPtr = std::move(persistentDataInteractorPtr);
    }

    bool ClientCheckerManager::checkClient(const std::string& data) const
    {
      return _persistentDataInteractorPtr->find(data);
    }
  }
}
