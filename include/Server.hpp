// -------------------------------------------------------------------------- //

#ifndef SERVER_HPP
# define SERVER_HPP

#include "UserManagement.hpp" // needed for UserManagement class
#include "ServerConfig.hpp"   // needed for MACROS
#include "typedefs.hpp"       // needed for Types and typedefs

#include <netinet/in.h> // needed for sockaddr_in
#include <csignal>	    // needed for signal handling

class Server{
    
    public:  /* constructors */

        Server();
        ~Server();

    public:  /* public variables */

        static bool serverRunning;

    public:  /* public member functions */

        void        start();
        void        getPortAndPasswd(char **argv); /* @note can possibly be done inside the class too */

		static void signal_handler(int sig); /* @note cant we handle signal inside the class ? */

    private: /* private variables */

        /* settings */
        int                      m_port;
        int                      m_maxClients;
        t_str                    m_passwd;

        /* connection */
        struct      sockaddr_in  address;
        int                      m_addrlen;
        int                      m_server_fd;
        t_vec_pollfd             m_pollfds;

        /* command parsing */
        t_vec_str                m_parameters;
        t_str                    m_command;
        t_str                    m_trail;

        /* user/channel handling */
        UserManagement um;

        /* server bot */
        struct ServerBot{
            int    socket;
            User * self;
        }                       Marvin;

    private: /* private member functions */

        void error         (t_str str);

        /* setup settings and connection */
        void comparePassword         ();
        void createBot();
        void acceptClients ();

        /* <------ server routine -----> */
        void routine();
        void receiveMessages (int i);
        void sendMessages    (int i);
        void cleanUpSockets();
        void cleanEmptyChannels();
        void autoPromoteOperator();

        /* command parsing */
        void    Messages                (int socket);
        void    checkCompleteMessage    (int socket);
        void    parseIncomingMessage    (t_str_c& incomingMessage, int socket);
        bool    checkUnallowedCharacters(t_str_c& strToCheck,
                                         t_str_c& unallowedChars)     const;
        t_str   getParameter            (t_str_c& message);
        void    getCommand              (t_str&   message);
        t_str_c getPartMessage()                                      const;

        /* user/channel handling */
        bool isErasable              (int socket)                     const;
        void eraseUserFromAllChannels(int socket);
        void createChannelBy         (int socket,
                                      t_str_c& channelName,
                                      t_str_c& channelKey);
        void addUserToChannels       (int socket,
                                      t_vec_str_c& channelNames,
                                      t_vec_str_c& channelKeys);
        void broadcast               (t_str_c& sender,
                                      t_str_c& channelName,
                                      t_str_c& nicknameKicked,
                                      t_str_c& message,
                                      t_str_c& command);

        /* log functions */
        void LOG      (t_str_c& message)              const;
        void LOG_INC  (int socket, t_str_c& message)  const;
        void LOG_SEND (int socket, t_str_c& message)  const;
        void LOG_ERR  (t_str_c& message)              const;
        void LOG_VEC  (t_str_c& name, t_vec_str_c& v) const;

        /* messages / commands */
        void CMD_CAP    (int socket);
        void CMD_NICK   (int socket);
        void CMD_USER   (int socket);
        void CMD_PING   (int socket);
        void CMD_QUIT   (int socket);
        void CMD_JOIN   (int socket);
        void CMD_PART   (int socket);
        void CMD_PRIVMSG(int socket);
        void CMD_TOPIC  (int socket);
        void CMD_INVITE (int socket);
        void CMD_KICK   (int socket);
        void CMD_MODE   (int socket);

        /* success replies */

        /* @note could add a server variable socketSender / socketTarget */
        void RPL_CAP            (int socket);
        void RPL_JOIN           (int socketSender,
                                 int socketTarget,
                                 t_str_c& channelName);
        void RPL_NAMREPLY       (int socket,
                                 t_str_c& channelName,
                                 t_str_c& members);
        void RPL_NICKCHANGE     (int socket,
                                 int socketTarget,
                                 t_str_c& newNickname,
                                 t_str_c& oldNickname);
        void RPL_NOTOPIC        (int socket, t_str_c& channelName);
        void RPL_PING           (int socket, t_str_c& serverName);
        void RPL_QUIT           (int socketSender,
                                 int socketTarget,
                                 t_str_c& message);
        void RPL_TOPIC          (int socket,
                                 t_str_c& channelName,
                                 t_str_c& topic);
        void RPL_WELCOME        (int socket, t_str_c& username);
        void RPL_PART           (int socketSender,
                                 int socketTarget,
                                 t_str_c& channelName,
                                 t_str_c& message);
        void RPL_KICK           (int socketSender,
                                 int socketTarget,
                                 t_str_c& channelName,
                                 t_str_c& nicknameKicked,
                                 t_str_c& message);
        void RPL_IFTOPIC        (int socket,
                                 t_str_c& channelName,
                                 t_str_c& topic);
        void RPL_PRIVMSG        (int socket,
                                 t_str_c& target,
                                 t_str_c& message);
        void RPL_PRIVMSG_CHANNEL(int socketSender,
                                 int socketTarget,
                                 t_str_c& channelName,
                                 t_str_c& message);
        void RPL_INVITING       (int socketSender,
                                 int socketTarget,
                                 t_str_c& channelName,
                                 t_str_c& target);
        void RPL_CHANNELMODEIS  (int socket,
                                 int socketTarget,
                                 t_str_c& channelName,
                                 t_str_c& modechar,
                                 t_str_c& parameter);

        /* error replies */

        /* @note prob dont have to pass the command */
        /* @note would benefit from server socketSender/socketTarget var too */
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
        void ERR_NOSUCHNICK      (int socket, t_str_c& nicknameOrChannelName);
        void ERR_USERONCHANNEL   (int socket,
                                  t_str_c& nickname,
                                  t_str_c& channelName);
        void ERR_NORECIPIENT     (int socket, t_str_c& command);
        void ERR_NOTEXTTOSEND    (int socket);
        void ERR_USERNOTINCHANNEL(int socketSender,
                                  int socketTarget,
                                  t_str_c& channelName);
        void ERR_UNKNOWNMODE     (int socket,
                                  char unknownChar,
                                  t_str_c& channelName);
        void ERR_MODEWRONGPARAM  (int socketSender,
                                  t_str_c& channelName,
                                  t_str_c& message);
        void ERR_UNKNOWNCOMMAND  (int socketSender);

};

#endif // SERVER_HPP

// -------------------------------------------------------------------------- //
