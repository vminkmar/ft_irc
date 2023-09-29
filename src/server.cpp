#include "server.hpp"
#include "UserManagement.hpp"
#include "user.hpp"
Server::Server() : m_maxClients(512) {}

Server::~Server(){};

void Server::createSocket() {
  // create the socket for the server
  
  this->m_addrlen = sizeof(address);
  if ((this->m_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    error("In socket");
  int reuseaddr = 1;
  if (setsockopt(m_server_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr,
                 sizeof(reuseaddr)) == -1)
    error("socketopt");
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);
  memset(address.sin_zero, '\0', sizeof(address.sin_zero));
  // bind the descriptor to a address family(AF_INET)
  if (bind(m_server_fd, reinterpret_cast<struct sockaddr *>(&address),
           sizeof(address)) < 0)
    error("In bind");
  // listen to client to accept it
  if (listen(m_server_fd, 10) < 0)
    error("listen");
  this->m_pollfds[0].fd = m_server_fd;
  this->m_pollfds->revents = POLLIN;
  while (1) {
    int ret = poll(m_pollfds, m_maxClients, -1);
    if (ret <= 0)
      perror("poll");
    receiveOnServer();
    receiveMessage();
  }
}

void Server::receiveOnServer() {
  if (m_pollfds[0].revents & POLLIN) {
    int newSocket;
    if ((newSocket = accept(m_server_fd, (struct sockaddr *)&address,
                            (socklen_t *)&m_addrlen)) < 0)
      error("accept");
    char buffer[30000] = {0};
    int reading = read(newSocket, buffer, 30000);
    reading = 0;
    std::cout << buffer << std::endl;
    memset(buffer, 0, sizeof(buffer));
    for (int j = 1; j < m_maxClients; j++) {
      if (m_pollfds[j].fd == -1) {
        m_pollfds[j].fd = newSocket;
        m_pollfds[j].revents = POLLIN;
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
      int valread = read(m_pollfds[i].fd, buffer, sizeof(buffer));
      valread = 0; //error
      parseIncomingMessage(buffer, m_pollfds[i].fd);
      memset(buffer, 0, sizeof(buffer));
    }
  }
}

void Server::sendResponse(int response, int socket) {

  this->m_pollfds[0].revents = POLLOUT;
  if (response == WELCOME) {
    std::string str = "001" + userManagement.getNick(socket) +
                      ":Welcome to the Internet Relay Network vminkmar";
    send(socket, str.c_str(), str.length(), 0);
  }
  this->m_pollfds[0].revents = POLLIN;
}

void Server::Messages(int socket) {
  if (m_command == "NICK") {
    userManagement.addUser(socket, "", "", OPERATOR);
    userManagement.setNick(socket, m_parameters);
    // sendResponse(WELCOME, socket);
  }
  if (m_command == "USER") {
		
    userManagement.print();
  }
}

void Server::parseIncomingMessage(char *buffer, int socket) {
  std::string message = buffer;
  getCommand(message);
  Messages(socket);
}

size_t wordCounter(std::string message) {
  size_t counter = 0;
  size_t end = message.find(" ");
  while (end != std::string::npos) {
    end = message.find(" ");
    message.erase(message.begin(), message.begin() + end + 1);
    counter++;
  }
  return counter;
}

void Server::printCommand() {
  if (!m_command.empty())
    std::cout << "Command: " << m_command << std::endl;
  if (!this->m_parameters.empty())
    std::cout << this->m_parameters << std::endl;
}

void Server::getCommand(std::string message) {
  size_t end = message.find(" ");
  this->m_command = message.substr(0, end);
  message.erase(message.begin(), message.begin() + end + 1);
  this->m_parameters = message;
  printCommand();
}

// void Server::getPortAndPasswd(char **argv) {
//   std::string str = argv[1];
//   for (size_t i = 0; i < str.size() - 1; i++)
//     if (isnumber(str[i]) == false)
//       error("Bad input as Port");
//   this->m_port = atoi(argv[1]);
// }

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
