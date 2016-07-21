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
      int size() const;
      void addConnection(const T&);
      void deleteConnection(const T&);
      void deleteConnectionIf(const std::function<bool(const T&)>&);
      const T& getConnectionIf(const std::function<bool(const T&)>&) const;
      void apply(const std::function<void(const T&)>&);
    };

    template <typename T>
    int ConnectionManager<T>::size() const { return _connections.size(); }
    
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
    const T& ConnectionManager<T>
    ::getConnectionIf(const std::function<bool(const T&)>& func) const
    {
      typename std::list<T>::iterator	it =
	std::find_if(_connections.cbegin(), _connections.cend(), func);

      if (it != _connections.cend())
	return *it;
    }
    
    template <typename T>
    void ConnectionManager<T>::apply(const std::function<void(const T&)>& func)
    {
      std::for_each(_connections.begin(), _connections.end(), func);
    }
  }
}

#endif /* !CONNECTION_MANAGER_HPP_ */
