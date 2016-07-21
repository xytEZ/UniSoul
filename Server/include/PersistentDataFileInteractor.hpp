#ifndef PERSISTENT_DATA_FILE_INTERACTOR_HPP_
# define PERSISTENT_DATA_FILE_INTERACTOR_HPP_

# include "AFile.hpp"
# include "IPersistentDataInteractor.hpp"

namespace Persistence
{
  namespace File
  {
    template <typename T>
    class PersistentDataFileInteractor : public IPersistentDataInteractor<T>
    { 
    private :
      FilePtr<T>	_file;
    
    public :
      PersistentDataFileInteractor(const FilePtr<T>&);
      virtual ~PersistentDataFileInteractor() = default;
      virtual void init();
      virtual T find(const std::string&) const;
      virtual void close();
    };

    template <typename T>
    PersistentDataFileInteractor<T>
    ::PersistentDataFileInteractor(const FilePtr<T>& file) :
      _file(std::move(file))
    {
    }

    template <typename T>
    void PersistentDataFileInteractor<T>::init() { }

    template <typename T>
    T PersistentDataFileInteractor<T>::find(const std::string& data) const
    {
      return _file->find(data);
    }

    template <typename T>
    void PersistentDataFileInteractor<T>::close() { }
  }
}

#endif /* !PERSISTENT_DATA_FILE_INTERACTOR_HPP_ */
