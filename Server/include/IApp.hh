#ifndef IAPP_HH_
# define IAPP_HH_

namespace App
{
  class IApp
  {
  public :
    virtual ~IApp() = default;
    virtual bool init() = 0;
    virtual bool run() = 0;
    virtual bool close() = 0;
  };
}

#endif /* !IAPP_HH_ */
