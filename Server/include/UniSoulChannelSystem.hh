#ifndef UNI_SOUL_CHANNEL_SYSTEM_HH_
# define UNI_SOUL_CHANNEL_SYSTEM_HH_

# include "IChannelSystem.hh"

namespace Communication
{
  class UniSoulChannelSystem : public IChannelSystem
  {
  public :
    UniSoulChannelSystem() { };
    virtual ~UniSoulChannelSystem() { };
  };
}

#endif /* !UNI_SOUL_CHANNEL_SYSTEM_HH_ */
