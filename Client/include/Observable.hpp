#ifndef OBSERVABLE_HPP_
# define OBSERVABLE_HPP_

# include <memory>
# include <set>
# include <utility>
# include <algorithm>

# include "IObserver.hpp"

namespace Observer
{
  template <typename T>
  class Observable
  {
  private :
    using ObserverPtr = std::shared_ptr<IObserver<T>>;
  
  private :
    std::set<ObserverPtr>	_observers;

  public :
    Observable() = default;
    virtual ~Observable() = default;
    void notifyObservers(const T&) const;
    void addObserver(const ObserverPtr&);
    void removeObserver(const ObserverPtr&);
  };
  
  template <typename T>
  void Observable<T>::notifyObservers(const T& arg) const
  {
    std::for_each(_observers.cbegin(),
		  _observers.cend(),
		  [&arg](const ObserverPtr& observerPtr) -> void
		  {
		    observerPtr->update(arg);
		  });
  }
  
  template <typename T>
  void Observable<T>::addObserver(const ObserverPtr& observerPtr)
  {
    _observers.insert(observerPtr);
  }
  
  template <typename T>
  void Observable<T>::removeObserver(const ObserverPtr& observerPtr)
  {
    _observers.erase(observerPtr);
  }
}

#endif /* !OBSERVABLE_HPP_ */
