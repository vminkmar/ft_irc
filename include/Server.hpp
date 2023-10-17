// -------------------------------------------------------------------------- //

#ifndef SERVER_HPP
# define SERVER_HPP

#include "UserManagement.hpp" // needed for UserManagement class

#include <vector>       // needed for std::vector
#include <string>       // needed for std::string
#include <poll.h>       // needed for poll_fd
#include <netinet/in.h> // needed for sockaddr_in

//IP_ADDRESS 127.0.0.1
#define PORT 6667
#define HOST "localhost"

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
        std::string getParameter(std::string message);
        void getCommand(std::string &message);
        // void getPortAndPasswd(char **argv); /* @note no implement. */

        void createSocket(); /* this function essent. runs the whole server */
        
        /* <------ setup -----> */
        void runServer();
        void error(std::string str);
        void socketClosed(int i);
        void acceptClients();
        void cleanUpSockets();

        /* <------ client communication -----> */
        void Messages(int socket);
        void checkCompleteMessage(int socket);
        void sendMessages(int i);
        void receiveMessages(int i);
        void parseIncomingMessage(std::string message, int socket);
        void comparePassword();

        /* <------ server commands -----> */
        void QUIT_RPL(int socket);
        void WELCOME_RPL(int socket);
        void CAP_RPL(int socket);
        void PING_RPL(int socket);
        void JOIN_RPL(int socket, std::string name);
        void NICKCHANGE_RPL(int socket, std::string newNick);

        bool checkUnallowedCharacters(std::string nickname);

        /* <------ server errors -----> */
        void ERR_NICKNAMEINUSE(int socket, std::string nick);
        void ERR_ERRONEUSNICKNAME(int socket, std::string nick);
        void ERR_NONICKNAMEGIVEN(int socket);
        void ERR_NEEDMOREPARAMS(int socket, std::string command);

        /* <------ else -----> */
        void printCommand();

        void log    (std::string message); /* YELLOW */
        void log_in (std::string message); /* CYAN */
        void log_out(std::string message); /* PINK */
        void log_err(std::string message); /* RED */

};

#endif // SERVER_HPP

// -------------------------------------------------------------------------- //
