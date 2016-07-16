#ifndef CHAT_ROOM_GETTER_MANAGER_HH_
# define CHAT_ROOM_GETTER_MANAGER_HH_

# include <memory>
# include <list>
# include <string>

namespace Communication
{
  namespace Chat
  {
    namespace Manager
    {
      class ChatRoomGetterManager
      {
      private :
	using StringList = std::list<std::string>;
	using PersistentDataInteractorPtr =
	  std::unique_ptr<Persistence::IPersistentDataInteractor<StringList>>;

      private :
	PersistentDataInteractorPtr	_persistentDataInteractorPtr;
		
      public :
	ChatRoomGetterManager(const PersistentDataInteractorPtr&);
	~ChatRoomGetterManager();
      };
    }
  }
}

#endid /* !CHAT_ROOM_GETTER_MANAGER_HH_ */
