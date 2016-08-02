#ifndef UNI_SOUL_CLIENT_HH_
# define UNI_SOUL_CLIENT_HH_

# include "ObserverOutputResult.hh"
# include "IModel.hpp"

namespace Model
{
  class UniSoulClient : public IModel<Observer::OutputResult>
  {
  public :
    UniSoulClient(const std::string&, unsigned short);
    virtual ~UniSoulClient();
    virtual void execute(const std::string&);

  private :
    void _init();
    void _close();
  };
}

#endif /* !UNI_SOUL_CLIENT_HH_ */
