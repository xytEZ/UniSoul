#ifndef SOCKET_MANAGER_HPP_
# define SOCKET_MANAGER_HPP_

# include <memory>
# include <list>
# include <algorithm>
# include <functional>

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
      SocketPtr findSocketPtrIf(const PredSocketPtr&);
      void apply(const FuncSocketPtr&);
    };
    
    template <typename T>
    void SocketManager<T>::addSocketPtr(const SocketPtr& socketPtr)
    {
      _socketsPtr.push_back(socketPtr);
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
    typename SocketManager<T>::SocketPtr
    SocketManager<T>::findSocketPtrIf(const PredSocketPtr& pred)
    {
      typename SocketsPtr::const_iterator	it
	= std::find_if(_socketsPtr.cbegin(), _socketsPtr.cend(), pred);

      return it != _socketsPtr.cend() ? *it : nullptr;
    }
    
    template <typename T>
    void SocketManager<T>::apply(const FuncSocketPtr& func)
    {
      std::for_each(_socketsPtr.begin(), _socketsPtr.end(), func);
    }
  }
}

#endif /* !SOCKET_MANAGER_HPP_ */
