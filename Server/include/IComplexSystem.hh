#ifndef ICOMPLEX_SYSTEM_HH_
# define ICOMPLEX_SYSTEM_HH_

namespace System
{
  struct IComplexSystem
  {
    virtual ~IComplexSystem() { };
    virtual void toString() const = 0;
  };

}

#endif /* !ICOMPLEX_SYSTEM_HH_ */
