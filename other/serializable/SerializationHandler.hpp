#ifndef SERIALIZATION_HANDLER_HH_
# define SERIALIZATION_HANDLER_HH_

# include <memory>
# include <string>
# include <sstream>
# include <boost/shared_ptr.hpp>
# include <boost/serialization/shared_ptr.hpp>
# include <boost/archive/binary_oarchive.hpp>
# include <boost/archive/binary_iarchive.hpp>
# include "SmartPointer.hpp"

namespace Serializable
{
  template <typename T>
  class ASerializable;
}

namespace Handler
{
  template <typename T>
  class SerializationHandler
  {
  public :
    SerializationHandler() = default;
    ~SerializationHandler() = default;
    
    std::string serialize(std::shared_ptr
			  <Serializable::ASerializable<T>>&) const;
    std::string serialize(std::shared_ptr
			  <Serializable::ASerializable<T>>&&) const;

    template <typename U>
    std::shared_ptr<U> deserialize(const std::string&) const;
  };

  template <typename T>
  std::string SerializationHandler<T>
  ::serialize(std::shared_ptr
	      <Serializable::ASerializable<T>>& serializablePtr) const
  {
    std::stringstream                                 ss;
    boost::archive::binary_oarchive                   oa(ss);
    boost::shared_ptr<Serializable::ASerializable<T>> serializablePtr2 =
      Tool::SmartPointer::make_boost_ptr_from_std(serializablePtr);

    oa << serializablePtr2;
    return ss.str();
  }

  template <typename T>
  std::string SerializationHandler<T>
  ::serialize(std::shared_ptr
	      <Serializable::ASerializable<T>>&& serializablePtr) const
  {
    return serialize(serializablePtr);
  }

  template <typename T>
  template <typename U>
  std::shared_ptr<U> SerializationHandler<T>
  ::deserialize(const std::string& serializedStr) const
  {
    std::stringstream                                 ss(serializedStr);
    boost::archive::binary_iarchive                   ia(ss);
    boost::shared_ptr<Serializable::ASerializable<T>> serializablePtr
      = nullptr;
    std::shared_ptr<Serializable::ASerializable<T>>   serializablePtr2
      = nullptr;

    ia >> serializablePtr;
    serializablePtr2 = Tool::SmartPointer
      ::make_shared_ptr_from_boost(serializablePtr);
    return std::dynamic_pointer_cast<U>(serializablePtr2);
  }
}

#endif /* !SERIALIZATION_HANDLER_HH_ */
