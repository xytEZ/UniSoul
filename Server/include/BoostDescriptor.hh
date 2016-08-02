#ifndef BOOST_DESCRIPTOR_HH_
# define BOOST_DESCRIPTOR_HH_

# if defined(linux) || defined(__linux)
#  include <boost/asio/posix/stream_descriptor.hpp>
typedef boost::asio::posix::stream_descriptor   Descriptor;
# elif defined(_WIN32) || defined(__WIN32__)
#  include <boost/asio/windows/stream_handle.hpp>
typedef boost::asio::windows::stream_handle     Descriptor;
# else
#  error "Can't config boost descriptor. Unknown operating system"
# endif

#endif /* !BOOST_DESCRIPTOR_HH_ */
