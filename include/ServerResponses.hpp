#ifndef SERVERRESPONSES_HPP
#define SERVERRESPONSES_HPP
#include <iostream>
#include <string>

#define SERVERNAME "irc"

enum Responses {
  WELCOME,
  CAP,
	PING,
	QUIT,
	JOIN,
	PASS,
};

namespace irc {
	std::string CAP_RPL(std::string &command);
	std::string WELCOME_RPL(std::string user, std::string nick);
}

#endif
