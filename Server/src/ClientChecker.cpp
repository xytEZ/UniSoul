#include "ClientChecker.hh"

namespace Persistence
{
  ClientChecker::ClientChecker(const PersistentDataInteractorPtr&
			       persistentDataInteractorPtr) :
    _persistentDataInteractorPtr(std::move(persistentDataInteractorPtr))
  {
  }
  
  void ClientChecker
  ::setPersistentDataInteractorPtr(const PersistentDataInteractorPtr&
				   persistentDataInteractorPtr)
  {
    _persistentDataInteractorPtr = std::move(persistentDataInteractorPtr);
  }
  
  bool ClientChecker::check(const std::string& data) const
  {
    return _persistentDataInteractorPtr->find(data);
  }
}

