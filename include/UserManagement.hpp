// -------------------------------------------------------------------------- //

#ifndef USERMANAGEMENT_HPP
#define USERMANAGEMENT_HPP

#include <string>  // needed for std::string
#include <map>     // needed for std::map
#include <vector>  // needed for std::vector

#include "Channel.hpp"       // needed for Channel class
#include "UserPrivilege.hpp" // needed for ENUM
#include "User.hpp"          // needed for User class
//#include "Responses.hpp" not needed right now

/* define maximum no. of channels? */

#define INPUT 1
#define OUTPUT 2

class UserManagement
{
    public:

        typedef std::map<int, User>            t_um_users;
        typedef t_um_users::iterator           t_um_users_it;
        typedef t_um_users::const_iterator     t_um_users_cit;

        typedef std::map<std::string, Channel> t_um_channels;
        typedef t_um_channels::iterator        t_um_channels_it;
        typedef t_um_channels::const_iterator  t_um_channels_cit;

    private:
		
        t_um_users    m_users;
        t_um_channels m_channels;
		
        bool checkForUser   (int socket)              const;
        bool checkForChannel(std::string channelName) const;
	
    public:

        /* <------ user operations-----> */

        void        setNick               (int socket, std::string newNickname);
        void        setUser               (int socket, std::string newUsername);
        std::string getNick               (int socket) const;
        std::string getUser               (int socket) const;
        std::string getNumberUsersAsString()           const;
        int         getNumberUsers        ()           const;
        std::string getUsernames          ()           const;
        void        addUser               (int socket,
                                           std::string const& nickname,
                                           std::string const& username);
        void        eraseUser             (int socket);
		void	    appendToBuffer(std::string message, int socket, int flag);
				void				eraseBuffer(int socket, int flag, int start, int end);
        std::string getBuffer(int socket, int flag);

        /* <------ channel operations-----> */

        void           addChannel          (std::string channelName);
        void           eraseChannel        (std::string channelName);
        Channel const& getChannel          (std::string channelName) const;
        std::string    getChannelNames     ()                        const;
        std::string    getChannelUsernames (std::string channelName) const;
        void           printChannelInfo    (std::string channelName) const;
        void           listChannels        ()                        const;
        void           addUserToChannel    (int socket,
                                            UserPrivilege up,
                                            std::string channelName);
        void           eraseUserFromChannel(int socket,
                                            std::string channelName);

        UserManagement();
        ~UserManagement();
};

#endif // USERMANAGEMENT_HPP

// -------------------------------------------------------------------------- //
