#ifndef CHANNEL_GETTER_MANAGER_HH_
# define CHANNEL_GETTER_MANAGER_HH_

# include <memory>
# include <list>
# include <string>

namespace Communication
{
  namespace Channel
  {
    namespace Manager
    {
      class ChannelGetterManager
      {
      private :
	using StringList = std::list<std::string>;
	using PersistentDataInteractorPtr =
	  std::unique_ptr<Persistence::IPersistentDataInteractor<StringList>>;

      private :
	PersistentDataInteractorPtr	_persistentDataInteractorPtr;
	
	
      public :
	ChannelGetterManager(const PersistentDataInteractorPtr&);
	~ChannelGetterManager();
      };
    }
  }
}

#endid /* !CHANNEL_GETTER_MANAGER_HH_ */
