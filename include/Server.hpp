#ifndef SERVER_HPP
#define SERVER_HPP

/* @note not needed? */
//#include <arpa/inet.h>
//#include <iostream> 
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <string>
//#include <unistd.h>
//#include <list>
//#include <sstream>
//#include "UserManagement.hpp"
//#include "User.hpp"
//#include "ServerResponses.hpp"
//#include <sys/socket.h>
//#include <sys/types.h>

#include "UserManagement.hpp" // needed for UserManagement class

#include <vector>       // needed for std::vector
#include <poll.h>       // needed for poll_fd

#include <string>       // needed for std::string
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

        /* <------ else -----> */
        void printCommand();

};

#endif // SERVER_HPP
