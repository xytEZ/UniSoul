#ifndef CHAT_ROOM_HH_
# define CHAT_ROOM_HH_

# include <string>

namespace Communication
{
  namespace Chat
  {
    class ChatRoom
    {
    private :
      std::string	_name;

    public :
      ChatRoom(const std::string&);
      ~ChatRoom();
    }
  }
}

#endif /* !CHAT_ROOM_HH_ */
