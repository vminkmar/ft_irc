#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::string getCommand(std::string &message) {
  size_t end = message.find(" ");
  std::string command = message.substr(0, end);
  message.erase(message.begin(), message.begin() + end + 1);
  return command;
  // this->parameters = message;
  // printCommand();
}
std::vector<std::string> getParameter(std::string &message) {
  std::vector<std::string> parameters;

    size_t colon = message.find(":");
    if (colon != std::string::npos) {
        std::string beforeColon = message.substr(0, colon);
        std::string afterColon = message.substr(colon + 1);
        // Split the part before ":" by spaces
        std::istringstream iss(beforeColon);
        std::string token;

        while (std::getline(iss, token, ' ')) {
            parameters.push_back(token);
        }
				r
        // Add the part after ":" as a single parameter
        parameters.push_back(":" + afterColon);
    } else {
        // No ":" found, split the entire message by spaces
        std::istringstream iss(message);
        std::string token;

        while (std::getline(iss, token, ' ')) {
            parameters.push_back(token);
        }
    }

    return parameters;
}

int main() {
  std::string str = "USER vminkmar 0 * :vminkmar hello";
  std::string trail;
  std::string Command;
  std::vector<std::string> parameters;

  std::cout << str << std::endl;
  Command = getCommand(str);
  std::cout << "Command: " <<Command << std::endl;
  parameters = getParameter(str);
  for (std::vector<std::string>::iterator it = parameters.begin();
       it != parameters.end(); it++)
    std::cout << "Param: " << *it << std::endl;
  return (0);
}
