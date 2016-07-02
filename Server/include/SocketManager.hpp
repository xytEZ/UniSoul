#ifndef SOCKET_MANAGER_HPP_
# define SOCKET_MANAGER_HPP_

# include <memory>
# include <list>

namespace Network
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
    _sockets.deleteSocket(socket);
  }
}

#endif /* !SOCKET_MANAGER_HPP_ */
