#ifndef SOCKET_MANAGER_HPP_
# define SOCKET_MANAGER_HPP_

# include <memory>
# include <list>
# include <algorithm>
# include <functional>

namespace Network
{
  namespace Manager
  {
    template <typename T = int>
    class SocketManager
    {
    private :
      std::list<T>	_sockets;

    public :
      SocketManager();
      ~SocketManager();
      int size() const;
      void addSocket(const T&);
      void deleteSocket(const T&);
      void apply(const std::function<void(const T&)>&);
    };

    template <typename T>
    SocketManager<T>::SocketManager() { }

    template <typename T>
    SocketManager<T>::~SocketManager() { }

    template <typename T>
    int SocketManager<T>::size() const { return _sockets.size(); }

    template <typename T>
    void SocketManager<T>::addSocket(const T& socket)
    {
      _sockets.push_back(socket);
    }
  
    template <typename T>
    void SocketManager<T>::deleteSocket(const T& socket)
    {
      _sockets.remove(socket);
    }

    template <typename T>
    void SocketManager<T>::apply(const std::function<void(const T&)>& func)
    {
      std::for_each(_sockets.begin(), _sockets.end(), func);
    }
  }
}

#endif /* !SOCKET_MANAGER_HPP_ */
