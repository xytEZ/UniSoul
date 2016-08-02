#ifndef CONNECTION_MANAGER_HPP_
# define CONNECTION_MANAGER_HPP_

# include <memory>
# include <list>
# include <algorithm>
# include <functional>
# include <utility>

namespace Network
{
  namespace Manager
  {
    template <typename T>
    class ConnectionManager
    {
    private :
      std::list<T>	_connections;

    public :
      ConnectionManager() = default;
      ~ConnectionManager() = default;
      void addConnection(const T&);
      void deleteConnection(const T&);
      void deleteConnectionIf(const std::function<bool(const T&)>&);
      bool findConnectionIf(const std::function<bool(const T&)>&) const;
      void apply(const std::function<void(const T&)>&);
    };
    
    template <typename T>
    void ConnectionManager<T>::addConnection(const T& connection)
    {
      _connections.push_back(std::move(connection));
    }
  
    template <typename T>
    void ConnectionManager<T>::deleteConnection(const T& connection)
    {
      _connections.remove(std::move(connection));
    }

    template <typename T>
    void ConnectionManager<T>
    ::deleteConnectionIf(const std::function<bool(const T&)>& func)
    {
      _connections.remove_if(func);
    }

    template <typename T>
    bool ConnectionManager<T>
    ::findConnectionIf(const std::function<bool(const T&)>& func) const
    {
      return std::find_if(_connections.cbegin(), _connections.cend(), func)
	!= _connections.cend();
    }
    
    template <typename T>
    void ConnectionManager<T>::apply(const std::function<void(const T&)>& func)
    {
      std::for_each(_connections.begin(), _connections.end(), func);
    }
  }
}

#endif /* !CONNECTION_MANAGER_HPP_ */
