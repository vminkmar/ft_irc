// -------------------------------------------------------------------------- //

#include "../include/Server.hpp"         // needed for Server class
#include "../include/UserManagement.hpp" // needed for UserManagement class

#include <unistd.h> // needed for read()
#include <cstring>  // needed for memset() (linux compilation)
#include <stdio.h>  // needed for perror() (linux compilation)
#include <cstdlib>  // needed for exit() (linux compilation)
#include <iostream> // needed for std::cout, std::endl
#include <sstream>  // needed for std::stringstream
#include <netdb.h>  // needed for addrinfo
#include <fcntl.h>  // needed for fcntl()

#include <arpa/inet.h> /*@note not needed in linux compilation */

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> constructors */

Server::Server() : m_maxClients(MAX_CLIENTS), m_command(""), m_trail(""){}
Server::~Server(){};
bool Server::serverRunning = true;

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> member functions */


void Server::createBot(){
	int botSocket = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serverAddress;
	struct addrinfo hints;
	struct addrinfo *servinfo;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if (getaddrinfo(HOST, itostr(PORT).c_str(), &hints, &servinfo) != 0) {
	    error("getaddrinfo");
	}

	serverAddress = *((struct sockaddr_in *)(servinfo->ai_addr));
	if (connect(botSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
	    error("In connect");
	}

	freeaddrinfo(servinfo);
}


void Server::start(){
  
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
	fcntl(m_server_fd, F_SETFL, O_NONBLOCK);
    struct pollfd newServer;
    newServer.fd = m_server_fd;
    newServer.events = POLLIN;
    m_pollfds.push_back(newServer);
	createBot();
    while (Server::serverRunning == true){
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
	close(m_server_fd);
	/* @note close bot fd*/
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
	fcntl(newSocket, F_SETFL, O_NONBLOCK);
    newClient.fd = newSocket;
    newClient.events = POLLIN | POLLOUT;
		newClient.revents = 0;
    this->m_pollfds.push_back(newClient);
	um.addUser(newSocket);
	if (m_pollfds.size() == 2){
		Marvin.socket = newSocket;
		Marvin.self = um.getUser(Marvin.socket);
		Marvin.self->setUsername("Bot");
		Marvin.self->setNickname("Marvin");
		Marvin.self->toggleWelcomedStatus();
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
            um.setOnlineStatus(it->fd, OFFLINE);
        }
        else if (it->revents & POLLIN){
            /* @note BUG: when just using /disconnect this condition is set */
            receiveMessages(it->fd);
        }
        else if (it->revents & POLLOUT){
            sendMessages(it->fd);
        }
        it->revents = 0;
    }
    cleanUpSockets();
}

bool Server::isErasable(int socket) const{
    if (um.checkForUser(socket) == true){
        if (um.getOnlineStatus(socket) == OFFLINE){
            return true;
        }
    }
    return false;
}

void Server::cleanUpSockets(){
    for(t_vec_pollfd_it it = m_pollfds.begin() + 2; it != m_pollfds.end();){
        int     socket = it->fd;
        t_str_c nickname = um.getNickname(it->fd);
        
        if (isErasable(socket) == true){
            um.eraseUser(socket);
            it = m_pollfds.erase(it);
            log("Socket #" + itostr(socket) + " has been removed ("
                + nickname + ")");
			close(socket);
        }
        else{
            ++it;
        }
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
    else if (m_command == "INVITE"){
        CMD_INVITE(socket);
    }
    else if (m_command == "KICK"){
        CMD_KICK(socket);
    }
    else if (m_command == "MODE"){
        CMD_MODE(socket);
	}
    else{
		ERR_UNKNOWNCOMMAND(socket);
    }
    // else if (m_command == "PASS")
    //     comparePassword();
    // }
    
    /* @note error prone, if you access empty channels afterwards */
    cleanEmptyChannels();
    autoPromoteOperator();

}

// void Server::comparePassword(){
// 	if(m_passwd == m_parameters[0])

// 	else
// 		throw sendError()
// 		writeToOutputBuffer(ERR_PASSWD)
// }

void Server::cleanEmptyChannels(){
    
    t_vec_str_c channelNames = split(um.getChannelNames(), ',');
    for (t_vec_str_cit it = channelNames.begin();
                       it != channelNames.end();
                       ++it){
        
        t_str_c channelName = *it;
        Channel const* channel = um.getChannel(channelName);
        if (channel->getNumberOfUsers() == 1){
            um.eraseChannel(channelName);
            log(channelName + " has been removed (no Users)");
        }
    }
}

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

void Server::parseIncomingMessage(t_str_c& incomingMessage, int socket){
  
    t_str message = incomingMessage;

	if (!um.getBuffer(socket, INPUT).empty())
    {
		t_str buffer = um.getBuffer(socket, INPUT);
        buffer.append(message);
        message = buffer;
    }
	log_inc(socket, incomingMessage);
    size_t pos = message.find("\r\n");
    if(pos != std::string::npos){
		log("<inputBuffer> " + um.getBuffer(socket, INPUT));
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
	}
    else {
		if(!incomingMessage.empty()){
        	um.appendToBuffer(socket, incomingMessage, INPUT);
		}
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

void Server::getCommand(t_str& message){
    size_t end = message.find(" ");
    this->m_command = message.substr(0, end);
    message.erase(message.begin(), message.begin() + end + 1);
}

void Server::error(t_str str){
    std::cerr << str << std::endl;
    exit(1);
}


Server::t_vec_str Server::split(t_str_c& parameter,
                                char delimiter) const{
    t_vec_str          split;
    t_str              token;
    std::stringstream  ss(parameter);

    while (std::getline(ss, token, delimiter)){
        split.push_back(token);
    }
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

void Server::broadcast(t_str_c& sender,
                       t_str_c& channelName,
                       t_str_c& nicknameKicked,
                       t_str_c& message,
                       t_str_c& command){
    
    int socketSender = um.getSocket(sender);
    Channel const* channel = um.getChannel(channelName);
    Channel::t_channel_users members = channel->getUserMap();

    for (Channel::t_channel_users_cit it = members.begin();
                                      it != members.end();
                                      ++it){
        t_str_c& nickname = um.getNickname(it->first);
        int socketTarget = um.getSocket(nickname);
        if (sender.empty() == false && sender == nickname){
            continue ;
        }
        if (command == "PRIVMSG"){
            RPL_PRIVMSG_CHANNEL(socketSender,
                                socketTarget,
                                channelName,
                                message);
        }
        else if (command == "QUIT"){
            RPL_QUIT(socketSender, socketTarget, message);
        }
        else if (command == "JOIN"){
            RPL_JOIN(socketSender, socketTarget, channelName);
        }
        else if (command == "PART"){
            RPL_PART(socketSender, socketTarget, channelName, message);
        }
        else if (command == "NAMREPLY"){
            RPL_NAMREPLY(socketTarget, channelName, um.getChannelNicknames(channelName));
        }
        else if (command == "KICK"){
            RPL_KICK(socketSender, socketTarget, channelName, nicknameKicked, message);
        }
        else if (command == "NICK"){
            RPL_NICKCHANGE(socketSender, socketTarget, um.getNickname(socketSender), nicknameKicked);
        }
        else if(command == "MODE"){
			RPL_CHANNELMODEIS(socketSender, socketTarget, channelName, message, nicknameKicked);
		}
    }
}

void Server::signal_handler(int sig){
	if(sig == SIGINT)
		Server::serverRunning = false;
	return ;
}

void Server::autoPromoteOperator()
{
    t_vec_str_c channelNames = split(um.getChannelNames(), ',');

    for (t_vec_str_cit it = channelNames.begin();
                       it != channelNames.end();
                       ++it){

        t_str_c& channelName = *it;
        Channel * channel = um.getChannel(*it);

        if (channel->hasOperator() == false){

            int socketFirstUser = channel->getFirstUserSocket();
            t_str_c& nicknameFirstUser = um.getNickname(socketFirstUser);

            channel->addUser(socketFirstUser, OPERATOR);

            t_str msg = t_str_c(DEFMSG_PROMOTION)
                        + "(Last Operator left the channel)";

            /* @note could let the bot broacast this message */
            broadcast(nicknameFirstUser,
                      channelName,
                      "",
                      msg,
                      "PRIVMSG");
        }
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
