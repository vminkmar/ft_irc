// -------------------------------------------------------------------------- //

#ifndef USERMANAGEMENT_HPP
# define USERMANAGEMENT_HPP

#include "Channel.hpp"       // needed for Channel class, UserPrivilege ENUM
#include "User.hpp"          // needed for User class

#include <map>    // needed for std::map
#include <string> // needed for std::string

#define INPUT  1
#define OUTPUT 2

class UserManagement{
    
    public:

        typedef std::map<int, User>        t_um_users;
        typedef t_um_users::iterator       t_um_users_it;
        typedef t_um_users::const_iterator t_um_users_cit;

        typedef std::map<std::string, Channel> t_um_channels;
        typedef t_um_channels::iterator        t_um_channels_it;
        typedef t_um_channels::const_iterator  t_um_channels_cit;

    private:

        t_um_users    m_users;
        t_um_channels m_channels;

    public:

        /* <------ constructors-----> */
        UserManagement();
        ~UserManagement();

        /* <------ user map operations-----> */
        void addUser         (int socket);
        void eraseUser       (int socket);
        bool checkForUser   (int socket)       const;
        bool checkForUsername(std::string username) const;
        bool checkForNickname(std::string nickname) const;

            /* <---- setters */
            void setUsername    (int socket, std::string user);
            void setNickname    (int socket, std::string parameter);
            void setOnlineStatus(int socket, bool flag);
            void appendToBuffer (std::string message, int socket, int flag);
            void eraseBuffer    (int socket, int flag, int start, int end);

            /* <---- getters */
            std::string getNickname    (int socket)           const;
            std::string getNicknames   ()                     const;
            std::string getUsername    (int socket)           const;
            std::string getUsernames   ()                     const;
            std::string getBuffer      (int socket, int flag) const;
            bool        getOnlineStatus(int socket)           const;

        /* <------ channel map operations-----> */
        void addChannel      (std::string name);
        void eraseChannel    (std::string name);
        bool checkForChannel (std::string name)        const;
        void printChannelInfo(std::string channelName) const;
        void listChannels    ()                        const;
            
            /* <---- setters */
            void addUserToChannel    (int socket,
                                      UserPrivilege up,
                                      std::string channelName);
            void eraseUserFromChannel(int socket, std::string channelName);
            
            /* <---- getters */
            Channel const& getChannel(std::string name)                 const;
            std::string    getChannelNames()                            const;
            std::string    getChannelUsernames(std::string channelName) const;
            std::string    getChannelNicknames(std::string channelName) const;

};

#endif // USERMANAGEMENT_HPP

// -------------------------------------------------------------------------- //
