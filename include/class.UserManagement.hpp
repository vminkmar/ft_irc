// -------------------------------------------------------------------------- //

#ifndef CLASS_USERMANAGEMENT_HPP
# define CLASS_USERMANAGEMENT_HPP

#include "class.Channel.hpp" // needed for Channel class, UserPrivilege ENUM
#include "class.User.hpp"    // needed for User class

#define INPUT  1
#define OUTPUT 2

#define OFFLINE false
#define ONLINE  true

class UserManagement{
    
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
        bool checkForUser    (int socket)        const;
        bool checkForUsername(t_str_c& username) const;
        bool checkForNickname(t_str_c& nickname) const;

        /* <---- user map setters */
        void setUsername      (int socket, t_str_c& newUsername);
        void setNickname      (int socket, t_str_c& newNickname);
        void setOnlineStatus  (int socket, bool flag);
        void setWelcomedStatus(int socket, bool flag);
		void setRegisteredStatus (int socket, bool flag);
        void appendToBuffer   (int socket, t_str_c& message, int flag);
        void eraseBuffer      (int socket, int start, int end, int flag);

        /* <---- user map getters */
        int    getSocket          (t_str_c& nickname)    const;
        t_str  getNickname        (int socket)           const;
        t_str  getNicknames       ()                     const;
        t_str  getUsername        (int socket)           const;
        t_str  getUsernames       ()                     const;
        t_str  getBuffer          (int socket, int flag) const;
        bool   getOnlineStatus    (int socket)           const;
        bool   getWelcomedStatus  (int socket)           const;
		User * getUser            (int socket);
		bool   getRegisteredStatus(int socket)          const;
        /* <------ channel map operations-----> */
        void addChannel      (t_str_c& channelName);
        void eraseChannel    (t_str_c& channelName);
        bool checkForChannel (t_str_c& channelName)  const;
        void printChannelInfo(t_str_c& channelName)  const;

        /* <---- channel map setters */
        void addUserToChannel    (int socket,
                                  UserPrivilege up,
                                  t_str_c& channelName);
        void eraseUserFromChannel(int socket, t_str_c& channelName);
        
        /* <---- channel map getters */
        Channel const* getChannel                 (t_str_c& channelName)  const;
        Channel *      getChannel                 (t_str_c& channelName);
        t_str          getChannelNames()                                  const;
        t_str          getChannelUsernames        (t_str_c& channelName)  const;
        t_str          getChannelNicknames        (t_str_c& channelName)  const;
		t_str          getChannelOperatorNicknames(t_str_c& channelName)  const;

};

#endif // CLASS_USERMANAGEMENT_HPP

// -------------------------------------------------------------------------- //
