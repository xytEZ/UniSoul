#ifndef UNI_SOUL_CONSOLE_VIEW_HH_
# define UNI_SOUL_CONSOLE_VIEW_HH_

# include "AView.hpp"

namespace View
{
  struct ViewState
  {
    bool	start;
    std::string	text;

    void set(bool, const std::string&);
  };
  
  class UniSoulConsoleView : public AView<ViewState>
  {
  private :
    static const constexpr char	*PROMPT_MESSAGE = "?> ";

  private :
    ViewState	_viewState;
    
  public :
    UniSoulConsoleView(const ControllerPtr&);
    virtual ~UniSoulConsoleView() = default;
    virtual void init();
    virtual void launch();
    virtual void display();
    virtual void close();
    virtual void update(const ViewState&);
  };
}

#endif /* !UNI_SOUL_CONSOLE_VIEW_HH_ */
