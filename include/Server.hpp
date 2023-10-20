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

class Server{
    
    private:

        // char buffer[1024]; /* @note needed ? */

        std::vector<int>         m_clients;    /* saves fd/sockets of clients */
        std::vector<std::string> m_parameters;
        std::vector<pollfd>      m_pollfds; /* holds server and client fd's */

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
        bool checkUnallowedCharacters(std::string const& nickname) const;

        /* <------ server commands -----> */
        void CMD_CAP (int socket);
        void CMD_NICK(int socket);

        /* <------ server replies -----> */
        void RPL_CAP       (int socket);
        void RPL_JOIN      (int socket);
        void RPL_NICKCHANGE(int socket, std::string const& newNickname);
        void RPL_QUIT      (int socket);
        void RPL_PING      (int socket);
        void RPL_WELCOME   (int socket);

        /* <------ server errors -----> */
        void ERR_NONICKNAMEGIVEN (int socket);
        void ERR_ERRONEUSNICKNAME(int socket, std::string const& nickname);
        void ERR_NICKNAMEINUSE   (int socket, std::string const& nickname);
        void ERR_NEEDMOREPARAMS  (int socket);
        void ERR_ALREADYREGISTRED(int socket);

        /* <------ else -----> */
        void printCommand();
        
        void log        (std::string const& message) const;
        void log_success(std::string const& message) const;
        void log_inc    (std::string const& message) const; 
        void log_send   (std::string const& message) const;
        void log_err    (std::string const& message) const;

};

#endif // SERVER_HPP

// -------------------------------------------------------------------------- //
