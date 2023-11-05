// -------------------------------------------------------------------------- //

#ifndef SERVER_HPP
# define SERVER_HPP

#include "UserManagement.hpp" // needed for UserManagement class
#include "ServerConfig.hpp"   // needed for MACROS
#include "typedefs.hpp"       // needed for Types and typedefs

#include <netinet/in.h> // needed for sockaddr_in
#include <csignal>	    // needed for signal handling

class Server{
    
    private:  /* Server Bot Marvin */

        struct Bot{
            int    socket;
            User * self;
        };

		struct Bot Marvin;

    private: /* Server variables */

        t_vec_str                m_parameters;
        t_vec_pollfd             m_pollfds;

        struct      sockaddr_in  address;

        int                      m_maxClients;
        int                      m_server_fd;
        int                      m_addrlen;
        int                      m_port;

        t_str                    m_command;
        t_str                    m_trail;
        t_str                    m_passwd;

    public:

        UserManagement um;

        /* <------ constructors -----> */
        Server();
        ~Server();

/* @note essentially all of this could be private at some point */
		void createBot();
		static bool serverRunning;
		static void signal_handler(int sig);

        /* <------ getters -----> */
        t_str getParameter(t_str_c& message);
        void  getCommand  (t_str&   message);
        // void getPortAndPasswd(char **argv); /* @note no implement. */

        void start(); /* this function essent. runs the whole server */
        
        /* <------ setup -----> */
        void runServer     ();
        void error         (t_str str);
        void acceptClients ();
        void cleanUpSockets();

        /* <------ client communication -----> */
        void Messages                (int socket);
        void checkCompleteMessage    (int socket);
        void sendMessages            (int i);
        void receiveMessages         (int i);
        void parseIncomingMessage    (t_str_c& incomingMessage, int socket);
        void comparePassword         ();
        bool checkUnallowedCharacters(t_str_c& strToCheck,
                                      t_str_c& unallowedChars) const;

        /* <------ server messages -----> */
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

        /* <------ server routines -----> */
        void cleanEmptyChannels();
        void autoPromoteOperator();

        /* <------ server messages helpers -----> */
        void      createChannelBy         (int socket,
                                           t_str_c& channelName,
                                           t_str_c& channelKey);
        void      addUserToChannels       (int socket,
                                           t_vec_str_c& channelNames,
                                           t_vec_str_c& channelKeys);
        void      eraseUserFromAllChannels(int socket);
        t_vec_str split                   (t_str_c& parameter,
                                           char delimiter)            const;
        t_str_c   sumParameters           (t_vec_str_cit start)       const;
        t_str_c   getPartMessage()                                    const;
        t_str_c   itostr                  (int i)                     const;
        bool      isErasable              (int socket)                const;
        void      broadcast               (t_str_c& sender,
                                           t_str_c& channelName,
                                           t_str_c& nicknameKicked,
                                           t_str_c& message,
                                           t_str_c& command);

        /* <------ server replies -----> */
        void RPL_CAP          (int socket);
        void RPL_JOIN         (int socketSender,
                               int socketTarget,
                               t_str_c& channelName);
        void RPL_NAMREPLY     (int socket,
                               t_str_c& channelName,
                               t_str_c& members);
        void RPL_NICKCHANGE   (int socket,
                               int socketTarget,
                               t_str_c& newNickname,
                               t_str_c& oldNickname);
        void RPL_NOTOPIC      (int socket, t_str_c& channelName);
        void RPL_PING         (int socket, t_str_c& serverName);
        void RPL_QUIT         (int socketSender,
                               int socketTarget,
                               t_str_c& message);
        void RPL_TOPIC        (int socket,
                               t_str_c& channelName,
                               t_str_c& topic);
        void RPL_WELCOME      (int socket, t_str_c& username);
        void RPL_PART         (int socketSender,
                               int socketTarget,
                               t_str_c& channelName,
                               t_str_c& message);
        void RPL_KICK         (int socketSender,
                               int socketTarget,
                               t_str_c& channelName,
                               t_str_c& nicknameKicked,
                               t_str_c& message);
        void RPL_IFTOPIC      (int socket,
                               t_str_c& channelName,
                               t_str_c& topic);
        void RPL_PRIVMSG      (int socket,
                               t_str_c& target,
                               t_str_c& message);
        void RPL_PRIVMSG_CHANNEL(int socketSender,
                                 int socketTarget,
                                 t_str_c& channelName,
                                 t_str_c& message);
        void RPL_INVITING     (int socketSender,
                               int socketTarget,
                               t_str_c& channelName,
                               t_str_c& target);
        void RPL_CHANNELMODEIS(int socket,
                               int socketTarget,
                               t_str_c& channelName,
                               t_str_c& modechar,
                               t_str_c& parameter);

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

        /* Server log functions */
        void LOG      (t_str_c& message)              const;
        void LOG_INC  (int socket, t_str_c& message)  const; 
        void LOG_SEND (int socket, t_str_c& message)  const;
        void LOG_ERR  (t_str_c& message)              const;
        void LOG_VEC  (t_str_c& name, t_vec_str_c& v) const;

};

#endif // SERVER_HPP

// -------------------------------------------------------------------------- //
