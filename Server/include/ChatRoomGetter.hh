#ifndef CHAT_ROOM_GETTER_HH_
# define CHAT_ROOM_GETTER_HH_

# include <memory>
# include <vector>
# include <string>

namespace Communication
{
  namespace Chat
  {
    class ChatRoomGetterManager
    {
    private :
      using StringArray = std::vector<std::string>;
      using PersistentDataInteractorPtr =
	std::unique_ptr<Persistence::IPersistentDataInteractor<StringArray>>;
      
    private :
      PersistentDataInteractorPtr	_persistentDataInteractorPtr;
      
    public :
      ChatRoomGetter(const PersistentDataInteractorPtr&);
      ~ChatRoomGetter();
    };
  }
}

#endid /* !CHAT_ROOM_GETTER_HH_ */
