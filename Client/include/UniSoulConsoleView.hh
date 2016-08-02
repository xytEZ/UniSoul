#ifndef UNI_SOUL_CONSOLE_VIEW_HH_
# define UNI_SOUL_CONSOLE_VIEW_HH_

# include "ObserverOutputResult.hh"
# include "AView.hpp"

namespace View
{
  class UniSoulConsoleView : public AView<Observer::OutputResult>
  { 
  private :
    static constexpr const char	*PROMPT_MESSAGE = "> ";

  private :
    bool		_start;
    std::string		_text;
    
  public :
    UniSoulConsoleView(const ControllerPtr&);
    virtual ~UniSoulConsoleView() = default;
    virtual void init();
    virtual void launch();
    virtual void display();
    virtual void close();
    virtual void update(const Observer::OutputResult&);
  };
}

#endif /* !UNI_SOUL_CONSOLE_VIEW_HH_ */
