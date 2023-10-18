// -------------------------------------------------------------------------- //

#include "../include/Server.hpp"         // needed for Server class
#include "../include/UserManagement.hpp" // needed for UserManagement class

#include <iostream> // needed for std::cout, std::endl
#include <sstream>  // needed for std::stringstream
#include <unistd.h> // needed for read()
#include <cstring>  // needed for memset() (linux compilation)
#include <stdio.h>  // needed for perror() (linux compilation)
#include <cstdlib>  // needed for exit() (linux compilation)

/* LOG COLOURING */
#define YELLOW "\033[33m"
#define PINK   "\033[95m"
#define CYAN   "\033[36m"
#define RED    "\033[31m"
#define WHITE  "\033[37m"
#define RESET  "\033[0m"

#define COLOUR_LOG     WHITE
#define COLOUR_SUCCESS WHITE
#define COLOUR_IN      CYAN
#define COLOUR_OUT     PINK
#define COLOUR_ERR     RED

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> constructors */

Server::Server() : m_maxClients(512), m_command(""), m_trail("") {}
Server::~Server(){};

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> member functions */

void Server::createSocket() {
  
    /* get sizeof of struct sockaddr_in address */
    this->m_addrlen = sizeof(this->address);

    /* set socket int to m_server_fd */
    if ((this->m_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        error("In socket");
    }

    int reuseaddr = 1;
    /* set socket options */
    if (setsockopt(m_server_fd,
                   SOL_SOCKET,
                   SO_REUSEADDR,
                   &reuseaddr,
                   sizeof(reuseaddr)) == -1){
        error("socketopt");
    }
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    /* use of conversion function htons (host byte -> network byte order) */
    this->address.sin_port = htons(PORT);
    memset(this->address.sin_zero, '\0', sizeof(this->address.sin_zero));

    /* bind address to socket */
    if (bind(this->m_server_fd,
             reinterpret_cast<struct sockaddr *>(&address),
             sizeof(this->address)) < 0){
        error("In bind");
    }

    /* makes socket ready to listen for clients */
    if (listen(this->m_server_fd, 10) < 0){
        error("listen");
    }

    struct pollfd newServer;
    newServer.fd = m_server_fd;
    newServer.events = POLLIN;
    m_pollfds.push_back(newServer);

    while (1){
        /* waits for event on filedescriptor */
        int ret = poll(this->m_pollfds.data(), m_pollfds.size(), 100);
        if (ret < 0){
            perror("poll"); /* @note perror instead of error? */
        }

        /* if there is some incoming event */
        if (this->m_pollfds[0].revents & POLLIN){
            acceptClients();
        }
        runServer();
    }
}

void Server::acceptClients(){
    
    /* accepts client on newSocket */
    int newSocket;
    if ((newSocket = accept(this->m_server_fd,
                     (struct sockaddr *)&address,
                     (socklen_t *)&m_addrlen)) < 0){
         error("accept");
    }

    struct pollfd newClient; 
    newClient.fd = newSocket;
    newClient.events = POLLIN | POLLOUT;
    this->m_pollfds.push_back(newClient);

    um.addUser(newSocket);
    this->m_pollfds[0].revents = 0; /* current event */
}

void Server::runServer(){
    for (std::vector<pollfd>::iterator it = m_pollfds.begin() + 1;
                                       it != m_pollfds.end();
                                       ++it){
        if (it->revents == 0){
            continue;
	    }
        if (it->revents & (POLLERR | POLLHUP | POLLNVAL)){
            socketClosed(it->fd);
        }
        if (it->revents & POLLIN){
            receiveMessages(it->fd);
        }
        if (it->revents & POLLOUT){
            sendMessages(it->fd);
        }
        it->revents = 0;
    }
    cleanUpSockets();
}

void Server::cleanUpSockets(){
	for(std::vector<pollfd>::iterator it = m_pollfds.begin() + 1;
                                      it != m_pollfds.end();){
		if(um.getOnlineStatus(it->fd) == false){
			it = m_pollfds.erase(it);
		}
		else
			++it; /* @note could just be in the for condition */
	}
}


void Server::socketClosed(int socket){
    um.setOnlineStatus(socket, false);
}

void Server::sendMessages(int socket){
    if (!um.getBuffer(socket, OUTPUT).empty()){
        while (!um.getBuffer(socket, OUTPUT).empty()){
            std::string message = um.getBuffer(socket, OUTPUT);
            

            std::stringstream ss;
            ss << socket;
            log_send(message.substr(0, message.find_first_of("\r"))
                    + " --> socket #"
                    + ss.str());

            int sending = send(socket, message.data(), message.length(), 0);
            if (sending < 0){
                /* @note error handling? */
            }
            size_t end = um.getBuffer(socket, OUTPUT).find("\r\n");
            if (end != std::string::npos){
            um.eraseBuffer(socket, OUTPUT, 0, end + 2);
            }
        }
    }
}

void Server::receiveMessages(int socket) {
    char buffer[30000];
    memset(buffer, 0, sizeof(buffer));
    int valread = read(socket, buffer, sizeof(buffer));
    (void)valread; /* for linux compilation! (unused var) */
    valread = 0;   // error
    std::string message = buffer;
    parseIncomingMessage(message, socket);
    memset(buffer, 0, sizeof(buffer));
}

void Server::Messages(int socket){
    
    /* @note switch case statement? */
    if (m_command == "CAP")
    {
        RPL_CAP(socket);
    }
    else if (m_command == "NICK")
    {
        if (m_parameters.empty() == true){
            ERR_NONICKNAMEGIVEN(socket);
        }
        else if (checkUnallowedCharacters(m_parameters[0]) == true){
            ERR_ERRONEUSNICKNAME(socket, m_parameters[0]);
        }
        else if (um.checkForNickname(m_parameters[0]) == true){
            ERR_NICKNAMEINUSE(socket, m_parameters[0]);
        }
        else{
            RPL_NICKCHANGE(socket, m_parameters[0]);
            um.setNickname(socket, m_parameters[0]);
        }

        /* @note Changed, i think this is more practical */
        /* @note Do we want to abstract this code into a CMD_NICK() function? */
       
       // if(this->um.getNickname(socket) == "") /*@note good practice? */
       // {
       //     this->um.setNickname(socket, this->m_parameters[0]);
       // }
       // else
       // {
       //     if(um.checkForNickname(this->m_parameters[0]) == false)
       //     {
       //         RPL_NICKCHANGE(socket, m_parameters[0]);
       //         um.setNickname(socket, this->m_parameters[0]);
       //     }
       //     else
       //     {
       //         ERR_NICKNAMEINUSE(socket, m_parameters[0]);
       //     }
       // }
    }
    else if (m_command == "USER")
    {
        this->um.setUsername(socket, this->m_parameters[0]);
        RPL_WELCOME(socket);
        /* @note will this be send everytime someone changes his USER name? */
    }
    else if (m_command == "PING")
    {
        RPL_PING(socket);
    }
    else if (m_command == "QUIT")
    {
        RPL_QUIT(socket);
    }
    else if (m_command == "JOIN")
    {
        m_parameters[0] = m_parameters[0].erase(0, 1);
        um.addChannel(m_parameters[0]);
        RPL_JOIN(socket, m_parameters[0]);
    }
  // else if (m_command == "PASS") {
  //   comparePassword();
  // }
}

// void Server::comparePassword(){
// 	if(m_passwd == m_parameters[0])

// 	else
// 		throw sendError()
// 		writeToOutputBuffer(ERR_PASSWD)
// }

bool Server::checkUnallowedCharacters(std::string nickname){
	const std::string unallowedChars = " !@#$%^&*()[]{}<>:;,/";
	for(size_t i = 0; i < unallowedChars.length(); ++i){
		size_t find = nickname.find(unallowedChars[i]);
		if(find != std::string::npos){
			return true;
        }
	}
	return false;
}

void Server::parseIncomingMessage(std::string message, int socket) {
  
    /* if buffer isnt empty, append message to buffer and use that as new msg */
    if (!um.getBuffer(socket, INPUT).empty())
    {
        std::string buffer = um.getBuffer(socket, INPUT);
        buffer.append(message);
        message = buffer;
    }

    /* @note might be error prone */
    log_inc(message.substr(0, message.find_last_not_of("\n") + 1));

    size_t pos = message.find("\r\n");
    while (pos != std::string::npos)
    {
        std::string tmp = message.substr(0, pos);

        getCommand(tmp);

        tmp = getParameter(tmp);

        log("<command> " + this->m_command);
        for (std::vector<std::string>::iterator it = this->m_parameters.begin();
                                                it != this->m_parameters.end();
                                                ++it){
            log("<param>   " + *it);
        }
        this->m_trail = tmp;
        log("<trail>   " + this->m_trail);

        message.erase(message.begin(), message.begin() + pos + 2);
        if (!(um.getBuffer(socket, INPUT).empty())){
            um.eraseBuffer(socket, INPUT, 0, pos + 2);
        }
        Messages(socket);
        this->m_parameters.clear();
        pos = message.find("\r\n");
    }
    if (!message.empty()) {
        um.appendToBuffer(message, socket, INPUT);
    }
}

std::string Server::getParameter(std::string message) {
  size_t colon = message.find(":");
  if (colon != std::string::npos) {
    std::string before = message.substr(0, colon);
    std::string after = message.substr(colon + 1);
    std::stringstream iss(before);
    std::string token;
    while (iss >> token) {
      m_parameters.push_back(token);
    }
    return (after);
  } else {
    std::stringstream iss(message);
    std::string token;
    while (iss >> token) {
      this->m_parameters.push_back(token);
    }
    return ("");
  }
}

void Server::printCommand() {
    if (!this->m_command.empty()){
        log("Command: " + this->m_command);
    }
}

void Server::getCommand(std::string &message) {
  size_t end = message.find(" ");
  this->m_command = message.substr(0, end);
  message.erase(message.begin(), message.begin() + end + 1);
}

void Server::error(std::string str) {
  std::cerr << str << std::endl;
  exit(1);
}

void Server::log(std::string message){
    std::cout << COLOUR_LOG << message << RESET << std::endl;
}

void Server::log_success(std::string message){
    std::cout << COLOUR_SUCCESS << "Success: " << message << RESET << std::endl;
}

void Server::log_inc(std::string message){
    std::cout << COLOUR_IN << "\nIncoming: " << message << RESET << std::endl;
}

void Server::log_send(std::string message){
    std::cout << COLOUR_OUT << "Sending: "<< message << RESET << std::endl;
}

void Server::log_err(std::string message){
    std::cout << COLOUR_ERR << "Error: " << message << RESET << std::endl;
}

// void Server::getPortAndPasswd(char **argv) {
//   std::string str = argv[1];
//   for (size_t i = 0; i < str.size() - 1; i++)
//     if (isnumber(str[i]) == false)
//       error("Bad input as Port");
//   this->m_port = atoi(argv[1]);
// 	std::string passwd = argv[2];
// 	m_passwd = passwd;
// }

// -------------------------------------------------------------------------- //
