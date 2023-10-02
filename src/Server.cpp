#include "Server.hpp"
#include "UserManagement.hpp"
#include "User.hpp"
Server::Server() : m_maxClients(512) {}

Server::~Server(){};

void Server::createSocket() {
  // create the socket for the server

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
  // bind the descriptor to a address family(AF_INET)
  if (bind(this->m_server_fd, reinterpret_cast<struct sockaddr *>(&address),
           sizeof(this->address)) < 0)
    error("In bind");
  // listen to client to accept it
  if (listen(this->m_server_fd, 10) < 0)
    error("listen");
  this->m_pollfds[0].fd = m_server_fd;
  this->m_pollfds->revents = POLLIN;
  while (1) {
    int ret = poll(this->m_pollfds, this->m_maxClients, -1);
    if (ret <= 0)
      perror("poll");
    receiveOnServer();
    receiveMessage();
  }
}

void Server::receiveOnServer() {
  if (this->m_pollfds[0].revents & POLLIN) {
    int newSocket;
    if ((newSocket = accept(this->m_server_fd, (struct sockaddr *)&address,
                            (socklen_t *)&m_addrlen)) < 0)
      error("accept");
    char buffer[30000] = {0};
    int reading = read(newSocket, buffer, 30000);
    reading = 0;
    std::cout << buffer << std::endl;
    memset(buffer, 0, sizeof(buffer));
    for (int j = 1; j < m_maxClients; j++) {
      if (this->m_pollfds[j].fd == -1) {
        this->m_pollfds[j].fd = newSocket;
        this->m_pollfds[j].revents = POLLIN;
        break;
      }
    }
    capabilityNegotiation(newSocket);
  }
}

void Server::receiveMessage() {
  for (int i = 1; i < this->m_maxClients; i++) {
    if (this->m_pollfds[i].revents & POLLIN) {

      char buffer[30000] = {0};
      int valread = read(this->m_pollfds[i].fd, buffer, sizeof(buffer));
      valread = 0; // error
      parseIncomingMessage(buffer, this->m_pollfds[i].fd);
      memset(buffer, 0, sizeof(buffer));
    }
  }
}

void Server::sendResponse(int response, int socket) {

  this->m_pollfds[0].revents = POLLOUT;
  if (response == WELCOME) {
    std::string str = "001" + this->userManagement.getNick(socket) +
                      ":Welcome to the Internet Relay Network vminkmar";
    send(socket, str.c_str(), str.length(), 0);
  }
  this->m_pollfds[0].revents = POLLIN;
}

void Server::Messages(int socket) {
  if (m_command == "NICK") {
    if (this->userManagement.checkForUser(socket) == false)
      this->userManagement.addUser(socket, "", "", OPERATOR);
    this->userManagement.setNick(socket, this->m_parameters[0]);
    // sendResponse(WELCOME, socket);
  } else if (m_command == "USER") {
    this->userManagement.setUser(socket, this->m_parameters[0]);
    this->userManagement.print();
  }
}

void Server::parseIncomingMessage(char *buffer, int socket) {
  std::string message = buffer;
  getCommand(message);
  std::cout << "Command: " << this->m_command << std::endl;
  message = getParameter(message);
  for (std::vector<std::string>::iterator it = this->m_parameters.begin();
       it != this->m_parameters.end(); it++) {
    std::cout << "param: " << *it << std::endl;
  }
  this->m_trail = message;
  std::cout << "trail: " << this->m_trail << std::endl;
  Messages(socket);
  this->m_parameters.clear();
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
    while (iss >> token)
      this->m_parameters.push_back(token);
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

void Server::capabilityNegotiation(int newSocket) {
  this->m_pollfds[0].revents = POLLOUT;
  char buffer1[100] = "CAP * LS :\r\n";
  int sending = send(newSocket, buffer1, sizeof(buffer1), 0);
  if (sending == -1)
    error("sending");
  memset(buffer1, 0, sizeof(buffer1));
  this->m_pollfds[0].revents = POLLIN;
}

// void Server::getPortAndPasswd(char **argv) {
//   std::string str = argv[1];
//   for (size_t i = 0; i < str.size() - 1; i++)
//     if (isnumber(str[i]) == false)
//       error("Bad input as Port");
//   this->m_port = atoi(argv[1]);
// }