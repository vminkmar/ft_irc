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
#include <sstream>

#include "UserManagement.hpp"
#include "User.hpp"
#include "Responses.hpp"
#define PORT 6667
//IP_ADDRESS 127.0.0.1
#define HOST "localhost"

class Server {
private:
  struct pollfd m_pollfds[512];
	struct sockaddr_in address;
  std::vector<int> m_clients;
	std::vector<std::string> m_parameters;
  // char buffer[1024];
  int m_maxClients;
  int m_server_fd;
  int m_addrlen;
  int m_port;
	std::string m_command;
	std::string m_trail;
	std::string m_passwd;

public:
    UserManagement userManagement;
  Server();
  ~Server();
  void error(std::string str);
  void createSocket();
  void getPortAndPasswd(char **argv);
	void parseIncomingMessage(char *buffer, int socket);
	void capabilityNegotiation(int newSocket);
	void getCommand(std::string &message);
	void printCommand();
	void Messages(int socket);
	void writeToOutputBuffer(int repsonse, int socket);
	void acceptClients();
	void runServer();
	void socketClosed(int i);
	void sendMessages(int i);
	void receiveMessages(int i);
	void cleanUpSockets();
	std::string getParameter(std::string message);
	void getTrial(std::string &message);
	void checkMessage(std::string &message);
};

#endif // SERVER_HPP
