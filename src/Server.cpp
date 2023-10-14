// -------------------------------------------------------------------------- //

#include "../include/Server.hpp"         // needed for Server class
#include "../include/UserManagement.hpp" // needed for UserManagement class

#include <iostream> // needed for std::cout, std::endl
#include <sstream>  // needed for std::stringstream
#include <unistd.h> // needed for read

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> constructors */

Server::Server() : m_maxClients(512), m_command(""), m_trail("") {}
Server::~Server(){};

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> member functions */

void Server::createSocket() {
  this->m_addrlen = sizeof(this->address);
  if ((this->m_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    error("In socket");
  int reuseaddr = 1;
  if (setsockopt(m_server_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr,
                 sizeof(reuseaddr)) == -1)
    error("socketopt");
  this->address.sin_family = AF_INET;
  this->address.sin_addr.s_addr = INADDR_ANY;
  this->address.sin_port = htons(PORT);
  memset(this->address.sin_zero, '\0', sizeof(this->address.sin_zero));
  if (bind(this->m_server_fd, reinterpret_cast<struct sockaddr *>(&address),
           sizeof(this->address)) < 0)
    error("In bind");
  if (listen(this->m_server_fd, 10) < 0)
    error("listen");
  struct pollfd newServer;
  newServer.fd = m_server_fd;
  newServer.events = POLLIN;
  m_pollfds.push_back(newServer);
  while (1) {
    int ret = poll(this->m_pollfds.data(), m_pollfds.size(), 100);
    if (ret < 0)
      perror("poll");
    if (this->m_pollfds[0].revents & POLLIN)
      acceptClients();
    runServer();
  }
}

void Server::acceptClients() {
  int newSocket;
  if ((newSocket = accept(this->m_server_fd, (struct sockaddr *)&address,
                          (socklen_t *)&m_addrlen)) < 0)
    error("accept");
  struct pollfd newClient;
  newClient.fd = newSocket;
  newClient.revents = -1;
  this->m_pollfds.push_back(newClient);
  userManagement.addUser(newSocket);
}

void Server::runServer() {
  for (std::vector<pollfd>::iterator it = m_pollfds.begin() + 1; it != m_pollfds.end(); ++it) {
    if (it->revents == 0){
			// std::cout << "test1" << std::endl;
		  continue;
		}
    // if (it->revents & (POLLERR | POLLHUP | POLLNVAL))
    //   socketClosed(it->fd);
    if (it->revents & POLLIN)
      receiveMessages(it->fd);
    if (it->revents & POLLOUT)
      sendMessages(it->fd);
    it->revents = 0;
		// std::cout << "test" << std::endl;
  }
  // cleanUpSockets
}

void Server::socketClosed(int socket) {
  userManagement.setOnlineStatus(socket, false);
}

void Server::sendMessages(int socket) {
  if (!userManagement.getBuffer(socket, OUTPUT).empty()) {
    while (!userManagement.getBuffer(socket, OUTPUT).empty()) {
      std::string message = userManagement.getBuffer(socket, OUTPUT);
      std::cout << message << std::endl;
      int sending = send(socket, message.data(), message.length(), 0);
      if (sending < 0)
        std::cout << "sending" << std::endl;
      size_t end = userManagement.getBuffer(socket, OUTPUT).find("\r\n");
      if (end != std::string::npos)
        userManagement.eraseBuffer(socket, OUTPUT, 0, end + 2);
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

void Server::CAP_RPL(int socket) {
  if (m_parameters[0] == "LS") {
    std::cout << "CAP message send to Socket: " << socket << std::endl;
    std::string str = "CAP * LS :cap reply...\r\n";
    userManagement.appendToBuffer(str, socket, OUTPUT);
  }
}

void Server::WELCOME_RPL(int socket) {
  std::cout << "Welcome message send to: " << userManagement.getUser(socket)
            << std::endl;
  std::string str = "001 " + userManagement.getNick(socket) +
                    " :Welcome to the ft_irc network " +
                    userManagement.getNick(socket) + "!" +
                    userManagement.getUser(socket) + "@" + HOST + "\r\n";
  userManagement.appendToBuffer(str, socket, OUTPUT);
}

void Server::PING_RPL(int socket) {
  std::cout << "PONG message send to: " << userManagement.getUser(socket)
            << std::endl;
  std::string str = " PONG :" + m_parameters[0] + "\r\n";
  userManagement.appendToBuffer(str, socket, OUTPUT);
}

void Server::QUIT_RPL(int socket) {
  std::string str = userManagement.getNick(socket) + "!" +
                    userManagement.getUser(socket) + "@" + "localhost" +
                    " QUIT :Goodbye!\r\n";
  userManagement.appendToBuffer(str, socket, OUTPUT);
}

void Server::JOIN_RPL(int socket, std::string name) {
  std::string str = userManagement.getNick(socket) + "!" +
                    userManagement.getUser(socket) + "@" + HOST + " JOIN " +
                    name + " * :" + userManagement.getUser(socket) + "\r\n";
  userManagement.appendToBuffer(str, socket, OUTPUT);
}

void Server::Messages(int socket) {
  if (m_command == "CAP") {
    CAP_RPL(socket);
  } else if (m_command == "NICK") {
    this->userManagement.setNick(socket, this->m_parameters[0]);
  } else if (m_command == "USER") {
    this->userManagement.setUser(socket, this->m_parameters[0]);
    WELCOME_RPL(socket);
  } else if (m_command == "PING") {
    PING_RPL(socket);
  } else if (m_command == "QUIT")
    QUIT_RPL(socket);
  else if (m_command == "JOIN") {
    m_parameters[0] = m_parameters[0].erase(0, 1);
    userManagement.addChannel(m_parameters[0]);
    JOIN_RPL(socket, m_parameters[0]);
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

void Server::parseIncomingMessage(std::string message, int socket) {
  if (!userManagement.getBuffer(socket, INPUT).empty()) {
    std::string buffer = userManagement.getBuffer(socket, INPUT);
    buffer.append(message);
    message = buffer;
  }
  size_t pos = message.find("\r\n");
  while (pos != std::string::npos) {
    std::string tmp = message;
    tmp = message.substr(0, pos);
    getCommand(tmp);
    std::cout << "Command: " << this->m_command << std::endl;
    tmp = getParameter(tmp);
    for (std::vector<std::string>::iterator it = this->m_parameters.begin();
         it != this->m_parameters.end(); it++) {
      std::cout << "param: " << *it << std::endl;
    }
    this->m_trail = tmp;
    std::cout << "trail: " << this->m_trail << std::endl;
    message.erase(message.begin(), message.begin() + pos + 2);
    if (!(userManagement.getBuffer(socket, INPUT).empty()))
      userManagement.eraseBuffer(socket, INPUT, 0, pos + 2);
    Messages(socket);
    this->m_parameters.clear();
    pos = message.find("\r\n");
  }
  if (!message.empty()) {
    userManagement.appendToBuffer(message, socket, INPUT);
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
  if (!this->m_command.empty())
    std::cout << "Command: " << this->m_command << std::endl;
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
