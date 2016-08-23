#ifndef COMMAND_TYPE_HH_
# define COMMAND_TYPE_HH_

namespace Command
{
  enum Type
    {
      NONE = 0,
      CREATE_CONNECTION = 1,
      DISCONNECT = 2,
      MESSAGE = 3,
      GET_USER_FROM = 4
    };
}

#endif /* !COMMAND_TYPE_HH_ */
