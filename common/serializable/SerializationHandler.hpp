#ifndef SERIALIZATION_HANDLER_HH_
# define SERIALIZATION_HANDLER_HH_

# include <cstddef>
# include <memory>
# include <string>
# include <sstream>
# include <iomanip>
# include <ios>
# include <iostream>
# include <boost/serialization/shared_ptr.hpp>
# include <boost/archive/binary_oarchive.hpp>
# include <boost/archive/binary_iarchive.hpp>
# include <boost/archive/archive_exception.hpp>
# include "SmartPointer.hpp"
# include "SerializationException.hh"

namespace Serializable
{
  template <typename T>
  class ASerializable;
}

namespace Handler
{
  template <typename T, int HEADER_LENGTH = 8>
  class SerializationHandler
  { 
  public :
    SerializationHandler() = default;
    ~SerializationHandler() = default;

    template <typename U>
    std::string serialize(std::shared_ptr
			  <Serializable::ASerializable<T>>&&) const;

    template <typename U>
    std::string serialize(std::shared_ptr
			  <Serializable::ASerializable<T>>&) const;
    
    std::shared_ptr<Serializable::ASerializable<T>>
    deserialize(const std::string&) const;
  };

  template <typename T, int HEADER_LENGTH>
  template <typename U>
  std::string SerializationHandler<T, HEADER_LENGTH>
  ::serialize(std::shared_ptr
	      <Serializable::ASerializable<T>>&& serializablePtr) const
  {
    return serialize<U>(serializablePtr);
  }
  
  template <typename T, int HEADER_LENGTH>
  template <typename U>
  std::string SerializationHandler<T, HEADER_LENGTH>
  ::serialize(std::shared_ptr
	      <Serializable::ASerializable<T>>& serializablePtr) const
  {
    boost::shared_ptr<Serializable::ASerializable<T>>	serializablePtr2 =
      Tool::SmartPointer
      ::make_boost_ptr_from_std(serializablePtr);
    std::ostringstream					headerStream;
    std::ostringstream					archiveStream;

    try
      {
	boost::archive::binary_oarchive	archive(archiveStream);
	
	archive.register_type(static_cast<U *>(nullptr));
	boost::serialization::void_cast_register
	  (static_cast<Serializable::ASerializable<T> *>(nullptr),
	   static_cast<U *>(nullptr));
	archive << serializablePtr2;
      }
    catch (const boost::archive::archive_exception& e)
      {
	std::cout << e.what() << std::endl;
	throw Exception::SerializationException(e.what());
      }
    headerStream << std::setw(HEADER_LENGTH)
		 << std::hex
		 << archiveStream.str().size();
    if (!headerStream || headerStream.str().size() != HEADER_LENGTH)
      throw Exception::SerializationException("Header serialization fail.");
    return headerStream.str() + archiveStream.str();
  }
  
  template <typename T, int HEADER_LENGTH>
  std::shared_ptr<Serializable::ASerializable<T>>
  SerializationHandler<T, HEADER_LENGTH>::deserialize(const std::string&
						      serializedStr) const
  {
    boost::shared_ptr<Serializable::ASerializable<T>>	serializablePtr
      = nullptr;
    std::shared_ptr<Serializable::ASerializable<T>>	serializablePtr2
      = nullptr;
    std::istringstream					headerStream
      (std::string(serializedStr, 0, HEADER_LENGTH));
    std::size_t						headerSize;

    if (!(headerStream >> std::hex >> headerSize))
      throw Exception::SerializationException("Header deserialization fail.");
    try
      {
	std::string			data(serializedStr, headerSize);
	std::istringstream		archiveStream(data);
	boost::archive::binary_iarchive	archive(archiveStream);
	
	archive >> serializablePtr;
      }
    catch (const boost::archive::archive_exception& e)
      {
	throw Exception::SerializationException(e.what());
      }
    serializablePtr2 = Tool::SmartPointer
      ::make_shared_ptr_from_boost(serializablePtr);
    return serializablePtr2;
  }
}

#endif /* !SERIALIZATION_HANDLER_HH_ */
