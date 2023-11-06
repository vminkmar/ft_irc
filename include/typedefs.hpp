// -------------------------------------------------------------------------- //

#ifndef TYPEDEFS_HPP
#define TYPEDEFS_HPP

// #include "class.User.hpp"    // needed for User class
// #include "class.Channel.hpp" // needed for Channel class
#include <map>    // needed for std::map <-- c11 would be unordered map
#include <poll.h> // needed for poll_fd
#include <string> // needed for std::string
#include <vector> // needed for std::vector

class Server;
class UserManagement;
class User;
class Channel;

/* used to describe channel users rights */
enum UserPrivilege
{
    USER,
    OPERATOR,
};

/* used to store bot information/data */
struct ServerBot
{
    int socket;
    User *self;
};

// ------------------ std::string --------------------------- //

/* used as default string */
typedef std::string t_str;
typedef t_str const t_str_c;
typedef t_str::const_iterator t_str_cit;

// ------------------ std::vectors --------------------------- //

/* used to store parsed parameters */
typedef std::vector<t_str> t_vec_str;
typedef t_vec_str const t_vec_str_c;
typedef t_vec_str::iterator t_vec_str_it;
typedef t_vec_str::const_iterator t_vec_str_cit;

/* used to store all accepted socket fd */
typedef std::vector<pollfd> t_vec_pollfd;
typedef t_vec_pollfd::iterator t_vec_pollfd_it;

// ------------------ std::maps --------------------------- //

/* used to store sockets and their User class */
typedef std::map<int, User> t_um_users;
typedef t_um_users::iterator t_um_users_it;
typedef t_um_users::const_iterator t_um_users_cit;

/* used to store channelNames and their Channel class */
typedef std::map<t_str, Channel> t_um_channels;
typedef t_um_channels::iterator t_um_channels_it;
typedef t_um_channels::const_iterator t_um_channels_cit;

/* used to store users and their privileges in a channel */
typedef std::map<int, UserPrivilege> t_channel_users;
typedef t_channel_users::iterator t_channel_users_it;
typedef t_channel_users::const_iterator t_channel_users_cit;

#endif // TYPEDEFS_HPP

// -------------------------------------------------------------------------- //