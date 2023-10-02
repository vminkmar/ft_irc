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

#include "UserManagement.hpp"
#include "User.hpp"
#include "Responses.hpp"

#define PORT 6667
//IP_ADDRESS 127.0.0.1


class Server {
private:
  std::vector<int> m_clients;
  struct pollfd m_pollfds[512];
	struct sockaddr_in address;
	UserManagement userManagement;
  // char buffer[1024];
  int m_maxClients;
  int m_server_fd;
  int m_addrlen;
  int m_port;
	std::string m_command;
	std::string m_parameters;
	std::string m_passwd;

public:
  Server();
  ~Server();
  void error(std::string str);
  void createSocket();
  void getPortAndPasswd(char **argv);
	void parseIncomingMessage(char *buffer, int socket);
	void capabilityNegotiation(int newSocket);
	void getCommand(std::string message);
	void printCommand();
	void Messages(int socket);
	void sendResponse(int repsonse, int socket);
	void receiveOnServer();
	void receiveMessage();

	void setNickname();


};

#endif // SERVER_HPP
