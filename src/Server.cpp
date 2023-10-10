
#include "../include/Server.hpp"
#include "../include/UserManagement.hpp"
// #include "../include/User.hpp"

Server::Server() : m_maxClients(512), m_command(""), m_trail("") {}

Server::~Server(){};

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
  this->m_pollfds[0].fd = m_server_fd;
  this->m_pollfds->revents = POLLIN;
  while (1) {
    int ret = poll(this->m_pollfds, this->m_maxClients, -1);
    if (ret <= 0)
      perror("poll");
    acceptClients();
    runServer();
  }
}

void Server::acceptClients() {
  if (this->m_pollfds[0].revents & POLLIN) {
    int newSocket;
    if ((newSocket = accept(this->m_server_fd, (struct sockaddr *)&address,
                            (socklen_t *)&m_addrlen)) < 0)
      error("accept");
    char buffer[30000] = {0};
    int reading = read(newSocket, buffer, 30000);
    (void)reading; /* for linux compilation (unused var) */
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
		userManagement.addUser(newSocket);
    // capabilityNegotiation(newSocket);
  }
}

void Server::runServer() {
  for (int i = 1; i < this->m_maxClients; i++) {
    if (this->m_pollfds[i].revents == 0)
      continue;
    if (this->m_pollfds[i].revents & POLLIN)
      receiveMessages(this->m_pollfds[i].fd);
    if (this->m_pollfds[i].revents & POLLOUT)
      sendMessages(m_pollfds[i].fd);
    // }
    // if (this->m_pollfds[i].revents & (POLLERR | POLLHUP | POLLNVAL))
    //   socketClosed(i);
  }
  // cleanUpSockets();
}

void Server::sendMessages(int socket) {
  int sending;
  if (!userManagement.getBuffer(socket, OUTPUT).empty()) {
    std::string temp = userManagement.getBuffer(socket, OUTPUT);
    const char *message = temp.c_str();
    sending = send(socket, message, sizeof(message), 0);
    if (sending < 0)
      std::cout << "sending" << std::endl;
    std::string tmp = userManagement.getBuffer(socket, OUTPUT);
    size_t end = tmp.find("\r\n");
    if (end != std::string::npos)
      userManagement.eraseBuffer(socket, OUTPUT, 0, end);
  }
  // clearBuffer();
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

void Server::writeToOutputBuffer(int response, int socket) {
  if (response == CAP && m_parameters[0] == "LS") {
    std::string str = "CAP * LS :cap reply...\r\n";
    userManagement.appendToBuffer(str, socket, OUTPUT);
  }
  if (response == WELCOME) {
    std::string str = "001 " + userManagement.getNick(socket) +
                      " :Welcome to the ft_irc network " +
                      userManagement.getNick(socket) + "!" +
                      userManagement.getUser(socket) + "@" + HOST + "\r\n";
    userManagement.appendToBuffer(str, socket, OUTPUT);
  }
}


void Server::Messages(int socket) {
  if (m_command == "CAP") {
    writeToOutputBuffer(CAP, socket);
  } else if (m_command == "NICK") {
    this->userManagement.setNick(socket, this->m_parameters[0]);
  } else if (m_command == "USER") {
    this->userManagement.setUser(socket, this->m_parameters[0]);
    writeToOutputBuffer(WELCOME, socket);
  }
}

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
    message.erase(message.begin(), message.begin() + pos + 1);
    if (!(userManagement.getBuffer(socket, INPUT).empty()))
      userManagement.eraseBuffer(socket, INPUT, 0, pos + 1);
    Messages(socket);
    this->m_parameters.clear();
    pos = message.find("\r\n");
  }
  checkMessage(message);
  if (!message.empty()) {
    userManagement.appendToBuffer(message, socket, INPUT);
  }
}

void Server::checkMessage(std::string &message) {
  if (message[0] == '\n')
    message.erase(0, 1);
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
      std::cout << token << std::endl;
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

void Server::capabilityNegotiation(int newSocket) {
  this->m_pollfds[0].revents = POLLOUT;
  char buffer1[100] = "CAP * LS :cap reply...\r\n";
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
