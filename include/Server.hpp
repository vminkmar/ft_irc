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

/* UNALLOWED CHARACTERS */
#define UNALLOWED_NICK " !@#$%^&*()[]{}<>:;,/"

/* DEFAULT MESSAGES */
#define DEFMSG_PART "Goodbye!"

#define MAX_CLIENTS 512

class Server{
    
    private:

        typedef std::vector<std::string>  t_vec_str;
        typedef t_vec_str::const_iterator t_vec_str_cit;

        typedef std::vector<pollfd>       t_vec_pollfd;
        typedef t_vec_pollfd::iterator    t_vec_pollfd_it;

        t_vec_str                m_parameters;
        std::vector<pollfd>      m_pollfds;

        struct      sockaddr_in  address;

        int         m_maxClients;
        int         m_server_fd;
        int         m_addrlen;
        int         m_port;

        std::string m_command;
        std::string m_trail;
        std::string m_passwd;

    public:
	
        UserManagement um;

        /* <------ constructors -----> */
        Server();
        ~Server();

        /* @note essentially all of this could be private at some point */

        /* <------ getters -----> */
        std::string getParameter(std::string const& message);
        void        getCommand  (std::string&       message);
        // void getPortAndPasswd(char **argv); /* @note no implement. */

        void createSocket(); /* this function essent. runs the whole server */
        
        /* <------ setup -----> */
        void runServer     ();
        void error         (std::string str);
        void socketClosed  (int i);
        void acceptClients ();
        void cleanUpSockets();

        /* <------ client communication -----> */
        void Messages                (int socket);
        void checkCompleteMessage    (int socket);
        void sendMessages            (int i);
        void receiveMessages         (int i);
        void parseIncomingMessage    (std::string message, int socket);
        void comparePassword         ();
        bool checkUnallowedCharacters(std::string const& strToCheck,
                                      std::string const& unallowedChars) const;

        /* <------ server commands -----> */
        void CMD_CAP (int socket);
        void CMD_NICK(int socket);
        void CMD_USER(int socket);
        void CMD_PING(int socket);
        void CMD_QUIT(int socket);
        void CMD_JOIN(int socket);
        void CMD_PART(int socket);

        /* <------ server replies -----> */
        void RPL_CAP       (int socket);
        void RPL_JOIN      (int socket,
                            std::string const& channelName);
        void RPL_NAMREPLY  (int socket,
                            std::string const& channelName,
                            std::string const& members);
        void RPL_NICKCHANGE(int socket, std::string const& newNickname);
        void RPL_NOTOPIC   (int socket, std::string const& channelName);
        void RPL_PING      (int socket, std::string const& serverName);
        void RPL_QUIT      (int socket);
        void RPL_TOPIC     (int socket,
                            std::string const& channelName,
                            std::string const& channelTopic);
        void RPL_WELCOME   (int socket, std::string const& username);
        void RPL_PART      (int socket,
                            std::string const& channelName,
                            std::string const& partMessage);

        /* <------ server errors -----> */
        void ERR_NOSUCHCHANNEL    (int socket, std::string const& channelName);
        void ERR_NONICKNAMEGIVEN  (int socket);
        void ERR_ERRONEUSNICKNAME (int socket, std::string const& nickname);
        void ERR_NICKNAMEINUSE    (int socket, std::string const& nickname);
        void ERR_NEEDMOREPARAMS   (int socket, std::string const& command);
        void ERR_ALREADYREGISTRED (int socket);
        void ERR_NOTONCHANNEL     (int socket, std::string const& channelName);
        void ERR_BADCHANNELKEY    (int socket, std::string const& channelName);
        void ERR_INVITEONLYCHAN   (int socket, std::string const& channelName);

        /* <------ server logs -----> */
        void log            (std::string const& message)                  const;
        void log_inc        (int socket, std::string const& message)      const; 
        void log_send       (int socket, std::string const& message)      const;
        void log_err        (std::string const& message)                  const;
        void log_vector     (std::string const& name, t_vec_str const& v) const;
        void log_interaction(int socket,
                             std::string const& message)                  const;

        /* <------ else -----> */
        void              printCommand()                              const;
        /* splits parameters by a delimiter into a std::vector<std::string> */
        t_vec_str         split        (std::string const& parameter,
                                        char delimiter)               const;
        /* summarizes parameters from given iterator to end as std::string */
        std::string const sumParameters(t_vec_str_cit start)          const;
        /* combines all parameters (except first) as std::string */
        std::string const getPartMessage()                            const;

};

#endif // SERVER_HPP

// -------------------------------------------------------------------------- //
