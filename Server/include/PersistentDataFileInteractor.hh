#ifndef PERSISTENT_DATA_FILE_INTERACTOR_HH_
# define PERSISTENT_DATA_FILE_INTERACTOR_HH_

# include "IPersistentDataInteractor.hh"

namespace Persistence
{
  class PersistentDataFileInteractor : public IPersistentDataInteractor
  {
  public :
    PersistentDataFileInteractor() { }
    virtual ~PersistentDataFileInteractor() { };
  };
}

#endif /* !PERSISTENT_DATA_FILE_INTERACTOR_HH_ */
