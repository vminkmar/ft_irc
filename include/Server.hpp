// -------------------------------------------------------------------------- //

#ifndef SERVER_HPP
# define SERVER_HPP

#include "UserManagement.hpp" // needed for UserManagement class

#include <vector>       // needed for std::vector
#include <string>       // needed for std::string
#include <poll.h>       // needed for poll_fd
#include <netinet/in.h> // needed for sockaddr_in

//IP_ADDRESS 127.0.0.1
#define PORT       6667
#define HOST       "localhost"
#define SERVERNAME std::string("Valhalla")

/* LOG COLOURING */
#define YELLOW             "\033[33m"
#define PINK               "\033[95m"
#define CYAN               "\033[36m"
#define RED                "\033[31m"
#define WHITE              "\033[37m"
#define RESET              "\033[0m"
#define COLOUR_LOG         WHITE
#define COLOUR_INTERACTION WHITE
#define COLOUR_IN          CYAN
#define COLOUR_OUT         PINK
#define COLOUR_ERR         RED

#define CHAR_UNALLOWED_NICK " !@#$%^&*()[]{}<>:;,/"
#define CHAR_ALLOWED_CHANNEL "#&+!"

/* DEFAULT MESSAGES */
#define DEFMSG_PART "Goodbye!"

#define MAX_CLIENTS 512

class Server{
    
    private:

        typedef std::string               t_str;
        typedef t_str const               t_str_c;

        typedef std::vector<t_str>        t_vec_str;
        typedef t_vec_str const           t_vec_str_c;
        typedef t_vec_str::const_iterator t_vec_str_cit;

        typedef std::vector<pollfd>       t_vec_pollfd;
        typedef t_vec_pollfd::iterator    t_vec_pollfd_it;

        t_vec_str    m_parameters;
        t_vec_pollfd m_pollfds;

        struct      sockaddr_in  address;

        int         m_maxClients;
        int         m_server_fd;
        int         m_addrlen;
        int         m_port;

        t_str m_command;
        t_str m_trail;
        t_str m_passwd;

    public:
	
        UserManagement um;

        /* <------ constructors -----> */
        Server();
        ~Server();

/* @note essentially all of this could be private at some point */

        /* <------ getters -----> */
        t_str getParameter(t_str_c& message);
        void  getCommand  (t_str&   message);
        // void getPortAndPasswd(char **argv); /* @note no implement. */

        void createSocket(); /* this function essent. runs the whole server */
        
        /* <------ setup -----> */
        void runServer     ();
        void error         (t_str str);
        void socketClosed  (int i);
        void acceptClients ();
        void cleanUpSockets();

        /* <------ client communication -----> */
        void Messages                (int socket);
        void checkCompleteMessage    (int socket);
        void sendMessages            (int i);
        void receiveMessages         (int i);
        void parseIncomingMessage    (t_str message, int socket);
        void comparePassword         ();
        bool checkUnallowedCharacters(t_str_c& strToCheck,
                                      t_str_c& unallowedChars) const;

        /* <------ server messages -----> */
        void CMD_CAP (int socket);
        void CMD_NICK(int socket);
        void CMD_USER(int socket);
        void CMD_PING(int socket);
        void CMD_QUIT(int socket);
        void CMD_JOIN(int socket);
        void CMD_PART(int socket);

        /* <------ server messages helpers -----> */
        void      createChannelBy         (int socket,
                                           t_str_c& channelName);
        void      addUserToChannels       (int socket,
                                           t_vec_str_c& channelNames,
                                           t_vec_str_c& channelKeys);
        void      eraseUserFromAllChannels(int socket);
        t_vec_str split                   (t_str_c& parameter,
                                           char delimiter)            const;
        t_str_c   sumParameters           (t_vec_str_cit start)       const;
        t_str_c   getPartMessage()                                    const;
        t_str_c   itostr                  (int i)                     const;
        void broadcast       (t_str_c& sender, t_str_c& channelName, t_str_c& message);

        /* <------ server replies -----> */
        void RPL_CAP       (int socket);
        void RPL_JOIN      (int socket, t_str_c& channelName);
        void RPL_NAMREPLY  (int socket, t_str_c& channelName, t_str_c& members);
        void RPL_NICKCHANGE(int socket, t_str_c& newNickname);
        void RPL_NOTOPIC   (int socket, t_str_c& channelName);
        void RPL_PING      (int socket, t_str_c& serverName);
        void RPL_QUIT      (int socket);
        void RPL_TOPIC     (int socket, t_str_c& channelName, t_str_c& topic);
        void RPL_WELCOME   (int socket, t_str_c& username);
        void RPL_PART      (int socket, t_str_c& channelName, t_str_c& message);
        void RPL_IFTOPIC   (int socket, t_str_c& channelName, t_str_c& topic);

        /* <------ server errors -----> */
        void ERR_NOSUCHCHANNEL   (int socket, t_str_c& channelName);
        void ERR_NONICKNAMEGIVEN (int socket);
        void ERR_ERRONEUSNICKNAME(int socket, t_str_c& nickname);
        void ERR_NICKNAMEINUSE   (int socket, t_str_c& nickname);
        void ERR_NEEDMOREPARAMS  (int socket, t_str_c& command);
        void ERR_ALREADYREGISTRED(int socket);
        void ERR_NOTONCHANNEL    (int socket, t_str_c& channelName);
        void ERR_BADCHANNELKEY   (int socket, t_str_c& channelName);
        void ERR_INVITEONLYCHAN  (int socket, t_str_c& channelName);
        void ERR_CHANNELISFULL   (int socket, t_str_c& channelName);
        void ERR_CHANOPRIVSNEEDED(int socket, t_str_c& channelName);
        void ERR_NOSUCHNICK      (int socket, t_str_c& nickname);
        void ERR_USERONCHANNEL   (int socket, t_str_c& nickname, t_str_c& channelName);
        void ERR_NORECIPIENT     (int socket, t_str_c& command);

        /* <------ server logs -----> */
        void log            (t_str_c& message)                  const;
        void log_inc        (int socket, t_str_c& message)      const; 
        void log_send       (int socket, t_str_c& message)      const;
        void log_err        (t_str_c& message)                  const;
        void log_vector     (t_str_c& name, t_vec_str_c& v)     const;

        /* <------ else -----> */
        void              printCommand()                        const;

};

#endif // SERVER_HPP

// -------------------------------------------------------------------------- //
