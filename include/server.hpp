#ifndef SERVER_HPP
#define SERVER_HPP
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include "user.hpp"
#define PORT 6667
//IP_ADDRESS 127.0.0.1


class Server {
private:
  std::vector<int> m_clients;
  struct pollfd m_pollfds[512];
	UserManagement userManagement;
  // char buffer[1024];
  int m_maxClients;
  int m_server_fd;
  int m_addrlen;
  int m_port;
	std::string m_passwd;

public:
  Server();
  ~Server();
  void error(std::string str);
  void createSocket();
  void getPortAndPasswd(char **argv);
	void parseIncomingMessage(char *buffer);
};

#endif