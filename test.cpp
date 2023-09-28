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

int wordCounter(std::string message) {
  int counter = 0;
  size_t end = message.find(" ");
  while (end != std::string::npos) {
    end = message.find(" ");
		message.erase(message.begin(), message.begin() + end + 1);
    counter ++;
  }
	std::cout << counter << std::endl;
	return counter;
}

void splitMessage(std::string message) {
  size_t i = 0;
  int counter = wordCounter(message);
  std::string Array[counter];
  size_t end = message.find(" ");
  while (end != std::string::npos) {
    Array[i] = message.substr(0, end);
		message.erase(message.begin(), message.begin() + end + 1);
    end = message.find(" ");
    i++;
  }
	i = 0;
	while(i < counter){
		std::cout << Array[i] << std::endl;
		i++;
	}
}

void parseIncomingMessage(char *buffer) {
  std::string message = buffer;
  // std::cout << message;
	splitMessage(message);
	
}


int main(int argc, char **argv){
	parseIncomingMessage(argv[1]);
}
