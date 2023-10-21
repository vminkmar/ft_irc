// -------------------------------------------------------------------------- //

#include "../include/Server.hpp"         // needed for Server class
#include "../include/UserManagement.hpp" // needed for UserManagement class

#include <iostream> // needed for std::cout, std::endl
#include <sstream>  // needed for std::stringstream
#include <unistd.h> // needed for read()
#include <cstring>  // needed for memset() (linux compilation)
#include <stdio.h>  // needed for perror() (linux compilation)
#include <cstdlib>  // needed for exit() (linux compilation)

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> constructors */

Server::Server() : m_maxClients(512), m_command(""), m_trail("") {}
Server::~Server(){};

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> member functions */

void Server::createSocket(){
  
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
            error("poll");
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
        else{
            ++it;
        }
    }
}

void Server::socketClosed(int socket){
    um.setOnlineStatus(socket, false);
}

void Server::sendMessages(int socket){
    
    while (um.getBuffer(socket, OUTPUT).empty() == false){
        
        std::string const& outputBuffer = um.getBuffer(socket, OUTPUT);
        size_t messageEnd = outputBuffer.find("\r\n");
        if (messageEnd == std::string::npos){
            log_err("no \r\n found in OUTPUT Buffer!");
            break ;
        }
        messageEnd += 2;
        std::string const& message = outputBuffer.substr(0, messageEnd);
        
        log_send(socket, message);
        int sending = send(socket, message.data(), message.length(), 0);
        if (sending < 0){
            log_err("Send return < 0?");
        }
        um.eraseBuffer(socket, OUTPUT, 0, messageEnd);
    }
}

void Server::receiveMessages(int socket){
    char buffer[30000];
    memset(buffer, 0, sizeof(buffer));
    int reading = read(socket, buffer, sizeof(buffer));
    if (reading < 0){
        log_err("Reading error in receiveMessages()");
    }
    std::string message = buffer;
    parseIncomingMessage(message, socket);
    memset(buffer, 0, sizeof(buffer)); /* @note unneccessary? */
}

void Server::Messages(int socket){
    
    if (m_command == "CAP"){
        CMD_CAP(socket);
    }
    else if (m_command == "NICK"){
        CMD_NICK(socket);
    }
    else if (m_command == "USER"){
        CMD_USER(socket);
    }
    else if (m_command == "PING"){
        CMD_PING(socket);
    }
    else if (m_command == "QUIT"){
        CMD_QUIT(socket);
    }
    else if (m_command == "JOIN"){
        CMD_JOIN(socket);
    }
    // else if (m_command == "PASS")
    //     comparePassword();
    // }
}

// void Server::comparePassword(){
// 	if(m_passwd == m_parameters[0])

// 	else
// 		throw sendError()
// 		writeToOutputBuffer(ERR_PASSWD)
// }

bool Server::checkUnallowedCharacters(std::string const& stringToCheck,
                                      std::string const& unallowedChars) const{
	for(size_t i = 0; i < unallowedChars.length(); ++i){
		size_t find = stringToCheck.find(unallowedChars[i]);
		if(find != std::string::npos){
			return true;
        }
	}
	return false;
}

void Server::parseIncomingMessage(std::string message, int socket){
  
    /* if buffer isnt empty, append message to buffer and use that as new msg */
    if (!um.getBuffer(socket, INPUT).empty())
    {
        std::string buffer = um.getBuffer(socket, INPUT);
        buffer.append(message);
        message = buffer;
    }

    log_inc(socket, message.substr(0, message.find("\r\n")));
    
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

std::string Server::getParameter(std::string const& message){
    size_t colon = message.find(":");
    if (colon != std::string::npos){
        std::string before = message.substr(0, colon);
        std::string after = message.substr(colon + 1);
        std::stringstream iss(before);
        std::string token;
        while (iss >> token){
            m_parameters.push_back(token);
        }
        return (after);
    }
    else{
        std::stringstream iss(message);
        std::string token;
        while (iss >> token){
        this->m_parameters.push_back(token);
        }
        return ("");
    }
}

void Server::printCommand(){
    if (!this->m_command.empty()){
        log("Command: " + this->m_command);
    }
}

void Server::getCommand(std::string& message){
    size_t end = message.find(" ");
    this->m_command = message.substr(0, end);
    message.erase(message.begin(), message.begin() + end + 1);
}

void Server::error(std::string str){
    std::cerr << str << std::endl;
    exit(1);
}

void Server::log(std::string const& message) const{
    std::cout << COLOUR_LOG << message << RESET << std::endl;
}

void Server::log_interaction(int socket, std::string const& message) const{
    (void) socket;
    std::cout << COLOUR_INTERACTION
              << message
              << RESET << std::endl;
}

void Server::log_inc(int socket, std::string const& message) const{
    std::stringstream ss;
    ss << socket;
    std::cout << COLOUR_IN
              << "\nIncoming: " << message << " <-- socket#" << ss.str()
              << RESET << std::endl;
}

void Server::log_send(int socket, std::string const& message) const{
    std::stringstream ss;
    ss << socket;
    std::cout << COLOUR_OUT
              << "Sending: "<< message << " --> socket#" << ss.str()
              << RESET << std::endl;
}

void Server::log_err(std::string const& message) const{
    std::cout << COLOUR_ERR << "Error: " << message << RESET << std::endl;
}

void Server::log_vector(std::string const& name,
                        std::vector<std::string> const& vec) const{
    
    std::cout << COLOUR_LOG << "vec<" + name + "> ";
    for (std::vector<std::string>::const_iterator it = vec.begin();
                                                  it != vec.end();
                                                  ++it){
        std::cout << "[" << *it << "] ";
    }
    std::cout << RESET << std::endl;

}

std::vector<std::string> Server::split(std::string parameter,
                                       char delimiter){
    std::vector<std::string> split;
    std::string              token;
    std::stringstream        ss(parameter);
    while (std::getline(ss, token, delimiter)){
        split.push_back(token);
    }
    return split;
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
