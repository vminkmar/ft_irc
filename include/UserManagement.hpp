// -------------------------------------------------------------------------- //

#ifndef USERMANAGEMENT_HPP
# define USERMANAGEMENT_HPP

#include <map>    // needed for std::map
#include <string> // needed for std::string

#include "Channel.hpp"       // needed for Channel class
#include "User.hpp"          // needed for User class
#include "UserPrivilege.hpp" // needed for ENUM
// #include "Responses.hpp" not needed right now

/* define maximum no. of channels? */

#define INPUT 1
#define OUTPUT 2

class UserManagement{
    
    public:

        typedef std::map<int, User> t_um_users;
        typedef t_um_users::iterator t_um_users_it;
        typedef t_um_users::const_iterator t_um_users_cit;

        typedef std::map<std::string, Channel> t_um_channels;
        typedef t_um_channels::iterator t_um_channels_it;
        typedef t_um_channels::const_iterator t_um_channels_cit;

    private:

        t_um_users m_users;
        t_um_channels m_channels;

        bool checkForUser(int socket) const;
        bool checkForChannel(std::string name) const;
				bool checkUsername(std::string username) const;
    public:

        /* <------ constructors-----> */
        UserManagement();
        ~UserManagement();

        /* <------ user operations-----> */
				bool checkNickname(std::string nickname) const;
        void setNickname(int socket, std::string parameter);
        void setUsername(int socket, std::string user);
        std::string getNickname(int socket) const;
        std::string getUsername(int socket) const;
        std::string getUsernames() const;
        std::string getNicknames() const; /* @note new, needs testing. */
        std::string getNumberUsersAsString() const;
        int getNumberUsers() const;
        void addUser(int socket);
        void eraseUser(int socket);
        void appendToBuffer(std::string message, int socket, int flag);
        void eraseBuffer(int socket, int flag, int start, int end);
        std::string getBuffer(int socket, int flag);
        int getSize() const;
        void setOnlineStatus(int socket, bool flag);
        bool getOnlineStatus(int socket) const;

        /* <------ channel operations-----> */
        void addChannel(std::string name);
        void eraseChannel(std::string name);
        Channel const &getChannel(std::string name) const;
        std::string getChannelNames() const;
        std::string getChannelUsernames(std::string channelName) const;
        void printChannelInfo(std::string channelName) const;
        void listChannels() const;
        void addUserToChannel(int socket, UserPrivilege up, std::string channelName);
        void eraseUserFromChannel(int socket, std::string channelName);

};

#endif // USERMANAGEMENT_HPP

// -------------------------------------------------------------------------- //
