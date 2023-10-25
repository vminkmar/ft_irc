// -------------------------------------------------------------------------- //

#include "../include/Server.hpp"         // needed for Server class
#include "../include/UserManagement.hpp" // needed for UserManagement class

#include <unistd.h>  // needed for read()
#include <cstring>   // needed for memset() (linux compilation)
#include <stdio.h>   // needed for perror() (linux compilation)
#include <cstdlib>   // needed for exit() (linux compilation)
#include <iostream>  // needed for std::cout, std::endl
#include <sstream>   // needed for std::stringstream

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> constructors */

Server::Server() : m_maxClients(MAX_CLIENTS), m_command(""), m_trail("") {}
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

    try{
			um.addUser(newSocket);
		}
		catch(std::exception &e){
			log_err(e.what());
		}
    this->m_pollfds[0].revents = 0; /* current event */
}

void Server::runServer(){
    for (t_vec_pollfd_it it = m_pollfds.begin() + 1;
                         it != m_pollfds.end();
                         ++it){
        if (it->revents == 0){
            continue;
	    }
        if (it->revents & (POLLERR | POLLHUP | POLLNVAL)){
            socketClosed(it->fd);
        }
        else if (it->revents & POLLIN){
            receiveMessages(it->fd);
        }
        else if (it->revents & POLLOUT){
            sendMessages(it->fd);
        }
        it->revents = 0;
    }
    cleanUpSockets();
}

void Server::cleanUpSockets(){
    for(t_vec_pollfd_it it = m_pollfds.begin() + 1; it != m_pollfds.end();){
        if(um.getOnlineStatus(it->fd) == false){
            it = m_pollfds.erase(it);
        }
        else{
            ++it;
        }
    }
}

void Server::socketClosed(int socket){
    try{
			um.setOnlineStatus(socket, false);
		}
		catch (std::exception &e){
			log_err(e.what());
		}
}

void Server::sendMessages(int socket){
    
    while (um.getBuffer(socket, OUTPUT).empty() == false){
        
        t_str_c& outputBuffer = um.getBuffer(socket, OUTPUT);
        size_t messageEnd = outputBuffer.find("\r\n");
        if (messageEnd == t_str::npos){
            log_err("no \r\n found in OUTPUT Buffer!");
            break ;
        }
        messageEnd += 2;
        t_str_c& message = outputBuffer.substr(0, messageEnd);
        
        log_send(socket, message);
        int sending = send(socket, message.data(), message.length(), 0);
        if (sending < 0){
            log_err("Send return < 0?");
        }
        um.eraseBuffer(socket, 0, messageEnd, OUTPUT);
    }
}

void Server::receiveMessages(int socket){
    char buffer[30000];
    memset(buffer, 0, sizeof(buffer));
    int reading = read(socket, buffer, sizeof(buffer));
    if (reading < 0){
        log_err("Reading error in receiveMessages()");
    }
    t_str message = buffer;
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
    else if (m_command == "PART"){
        CMD_PART(socket);
    }
    else if (m_command == "PRIVMSG"){
        CMD_PRIVMSG(socket);
    }
    else if (m_command == "TOPIC"){
        CMD_TOPIC(socket);
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

bool Server::checkUnallowedCharacters(t_str_c& stringToCheck,
                                      t_str_c& unallowedChars) const{
	for(size_t i = 0; i < unallowedChars.length(); ++i){
		size_t find = stringToCheck.find(unallowedChars[i]);
		if(find != t_str::npos){
			return true;
        }
	}
	return false;
}

void Server::parseIncomingMessage(t_str message, int socket){
  
    /* if buffer isnt empty, append message to buffer and use that as new msg */
    if (!um.getBuffer(socket, INPUT).empty())
    {
        t_str buffer = um.getBuffer(socket, INPUT);
        buffer.append(message);
        message = buffer;
    }

    log_inc(socket, message.substr(0, message.find("\r\n")));
    
    size_t pos = message.find("\r\n");
    while (pos != t_str::npos)
    {
        t_str tmp = message.substr(0, pos);

        getCommand(tmp);

        tmp = getParameter(tmp);

        log("<command> " + m_command);
        for (t_vec_str_cit it = m_parameters.begin();
                           it != m_parameters.end();
                           ++it){
            log("<param>   " + *it);
        }
        m_trail = tmp;
        log("<trail>   " + m_trail);

        message.erase(message.begin(), message.begin() + pos + 2);
        if (!(um.getBuffer(socket, INPUT).empty())){
            um.eraseBuffer(socket, 0, pos + 2, INPUT);
        }
        Messages(socket);
        m_parameters.clear();
        pos = message.find("\r\n");
    }
    if (!message.empty()){
        um.appendToBuffer(socket, message, INPUT);
    }
}

Server::t_str Server::getParameter(t_str_c& message){
    size_t colon = message.find(":");
    if (colon != t_str::npos){
        t_str before = message.substr(0, colon);
        t_str after = message.substr(colon + 1);
        std::stringstream iss(before);
        t_str token;
        while (iss >> token){
            m_parameters.push_back(token);
        }
        return (after);
    }
    else{
        std::stringstream iss(message);
        t_str token;
        while (iss >> token){
            this->m_parameters.push_back(token);
        }
        return ("");
    }
}

void Server::printCommand() const{
    if (!this->m_command.empty()){
        log("Command: " + this->m_command);
    }
}

void Server::getCommand(t_str& message){
    size_t end = message.find(" ");
    this->m_command = message.substr(0, end);
    message.erase(message.begin(), message.begin() + end + 1);
}

void Server::error(t_str str){
    std::cerr << str << std::endl;
    exit(1);
}

void Server::log(t_str_c& message) const{
    std::cout << COLOUR_LOG << message << RESET << std::endl;
}

void Server::log_inc(int socket, t_str_c& message) const{
    std::cout << COLOUR_IN
              << "\nIncoming: " << message << " <-- socket#" << itostr(socket)
              << RESET << std::endl;
}

void Server::log_send(int socket, t_str_c& message) const{
    std::cout << COLOUR_OUT
              << "Sending: "<< message << " --> socket#" << itostr(socket)
              << RESET << std::endl;
}

void Server::log_err(t_str_c& message) const{
    std::cout << COLOUR_ERR << "Error: " << message << RESET << std::endl;
}

void Server::log_vector(t_str_c& name, t_vec_str_c& v) const{
    std::cout << COLOUR_LOG << "<" + name + "> ";
    for (t_vec_str_cit it = v.begin(); it != v.end(); ++it){
        std::cout << "[" << *it << "] ";
    }
    std::cout << RESET << std::endl;
}

Server::t_vec_str Server::split(t_str_c& parameter,
                                char delimiter) const{
    t_vec_str          split;
    t_str              token;
    std::stringstream  ss(parameter);

    while (std::getline(ss, token, delimiter)){
        split.push_back(token);
    }

    log_vector("splitted vec", split);
    return split;
}

Server::t_str_c Server::sumParameters(t_vec_str_cit start) const{
    if (start == m_parameters.end()){
        throw std::runtime_error("sum_parameter: Wrong iterator given!");
    }

    std::stringstream ss;
    t_vec_str_cit last = m_parameters.end() - 1;
    for (t_vec_str_cit it = start; it != m_parameters.end(); ++it){
        ss << *it;
        if (it != last){
            ss << " ";
        }
    }
    return ss.str();
}

Server::t_str_c Server::getPartMessage() const{
    if (m_parameters.size() >= 2){
        return sumParameters(m_parameters.begin() + 1);
    }
    return DEFMSG_PART;
}

Server::t_str_c Server::itostr(int i) const{
    std::stringstream ss;
    ss << i;
    return ss.str();
}

void Server::broadcast(t_str_c& sender, t_str_c& channelName, t_str_c& message){
    
    int socketSender = um.getSocket(sender);
    Channel const* channel = um.getChannel(channelName);
    Channel::t_channel_users members = channel->getUserMap();

    for (Channel::t_channel_users_cit it = members.begin(); it != members.end(); ++it){
        t_str_c& nickname = um.getNickname(it->first);
        if (sender.empty() == false && sender == nickname){
            continue ;
        }
        RPL_PRIVMSG(socketSender, nickname, message);
    }
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
