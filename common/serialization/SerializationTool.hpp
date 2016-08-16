#ifndef SERIALIZATION_TOOL_HPP_
# define SERIALIZATION_TOOL_HPP_

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
# include "SerializationFailException.hh"

namespace Serialization
{
  namespace Tool
  {
    template <typename T>
    std::string serialize(T& serializable)
    {
      std::ostringstream	archiveStream(std::stringstream::out
					      | std::stringstream::binary);

      try
	{
	  boost::archive::binary_oarchive	archive
	    (archiveStream, boost::archive::no_header);
        
	  archive << serializable;
	}
      catch (const boost::archive::archive_exception& e)
	{
	  std::cout << e.what() << std::endl;
	  throw Exception::Serialization::SerializationFail(e.what());
	}
      return archiveStream.str();
    }

    template <typename T>
    std::string serialize(T&& serializable)
    {
      return serialize<T>(serializable);
    }
  
    template <typename T>
    T deserialize(const std::string& str)
    {
      T	serializable;
      
      try
	{
	  std::istringstream		archiveStream
	    (str, std::stringstream::in | std::stringstream::binary);
        
	  boost::archive::binary_iarchive	archive
	    (archiveStream, boost::archive::no_header);

	  archive >> serializable;
	}
      catch (const boost::archive::archive_exception& e)
	{
	  throw Exception::Serialization::SerializationFail(e.what());
	}
      return serializable;
    }
  }
}

#endif /* !SERIALIZATION_TOOL_HPP_ */
