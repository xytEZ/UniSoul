#ifndef SOCKET_MANAGER_HPP_
# define SOCKET_MANAGER_HPP_

# include <iostream>
# include <memory>
# include <map>
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
      using PairSocketPtr = std::pair<std::string, SocketPtr>;
      using PredPairSocketPtr = std::function<bool(const PairSocketPtr&)>;
      using FuncPairSocketPtr = std::function<void(const PairSocketPtr&)>;
      using SocketsPtr = std::map<std::string, SocketPtr>;
      
    private :
      SocketsPtr	_socketsPtr;

    public :
      SocketManager() = default;
      ~SocketManager() = default;
      void addSocketPtr(const std::string&, const SocketPtr&);
      void deleteSocketPtr(const std::string&);
      void deleteSocketPtrIf(const PredPairSocketPtr&);
      bool findSocketPtrIf(const PredPairSocketPtr&) const;
      void apply(const FuncPairSocketPtr&);
    };
    
    template <typename T>
    void SocketManager<T>::addSocketPtr(const std::string& name,
					const SocketPtr& socketPtr)
    {
      _socketsPtr[name] = std::move(socketPtr);
    }
  
    template <typename T>
    void SocketManager<T>::deleteSocketPtr(const std::string& name)
    {
      _socketsPtr.erase(name);
    }

    template <typename T>
    void SocketManager<T>::deleteSocketPtrIf(const PredPairSocketPtr& pred)
    {
      typename SocketsPtr::iterator	begin = _socketsPtr.begin();
      typename SocketsPtr::iterator	end = _socketsPtr.end();
      
      while ((begin = std::find_if(begin, end, pred)) != end)
	_socketsPtr.erase(begin++);
    }
    
    template <typename T>
    bool SocketManager<T>::findSocketPtrIf(const PredPairSocketPtr& pred) const
    {
      return std::find_if(_socketsPtr.cbegin(), _socketsPtr.cend(), pred)
	!= _socketsPtr.cend();
    }
    
    template <typename T>
    void SocketManager<T>::apply(const FuncPairSocketPtr& func)
    {
      std::for_each(_socketsPtr.begin(), _socketsPtr.end(), func);
    }
  }
}

#endif /* !SOCKET_MANAGER_HPP_ */
