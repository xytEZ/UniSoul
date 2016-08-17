#ifndef SOCKET_MANAGER_HPP_
# define SOCKET_MANAGER_HPP_

# include <iostream>
# include <memory>
# include <list>
# include <algorithm>
# include <functional>
# include <utility>

namespace Network
{
  template <typename T>
  class ISocket;
  
  namespace Manager
  {
    template <typename T>
    class SocketManager
    {
    private :
      using SocketPtr = std::shared_ptr<Network::ISocket<T>>;
      using PredSocketPtr = std::function<bool(const SocketPtr&)>;
      using FuncSocketPtr = std::function<void(const SocketPtr&)>;
      using SocketsPtr = std::list<SocketPtr>;
      
    private :
      SocketsPtr	_socketsPtr;

    public :
      SocketManager() = default;
      ~SocketManager() = default;
      void addSocketPtr(const SocketPtr&);
      void deleteSocketPtr(const SocketPtr&);
      void deleteSocketPtrIf(const PredSocketPtr&);
      bool findSocketPtrIf(const PredSocketPtr&) const;
      void apply(const FuncSocketPtr&);
    };
    
    template <typename T>
    void SocketManager<T>::addSocketPtr(const SocketPtr& socketPtr)
    {
      _socketsPtr.push_back(std::move(socketPtr));
    }
  
    template <typename T>
    void SocketManager<T>::deleteSocketPtr(const SocketPtr& socketPtr)
    {
      _socketsPtr.remove(socketPtr);
    }

    template <typename T>
    void SocketManager<T>::deleteSocketPtrIf(const PredSocketPtr& pred)
    {
      _socketsPtr.remove_if(pred);
    }
    
    template <typename T>
    bool SocketManager<T>::findSocketPtrIf(const PredSocketPtr& pred) const
    {
      return std::find_if(_socketsPtr.cbegin(), _socketsPtr.cend(), pred)
	!= _socketsPtr.cend();
    }
    
    template <typename T>
    void SocketManager<T>::apply(const FuncSocketPtr& func)
    {
      std::for_each(_socketsPtr.begin(), _socketsPtr.end(), func);
    }
  }
}

#endif /* !SOCKET_MANAGER_HPP_ */
