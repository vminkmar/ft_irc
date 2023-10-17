// -------------------------------------------------------------------------- //

#include "../include/ServerResponses.hpp"

#include <iostream> // needed for std::cout, std::endl

#define HOST "localhost"

namespace irc{

    std::string CAP_RPL(std::string &command){
        std::cout << command << ": Capability negotiation was sent" << std::endl;
        return (std::string(":") + SERVERNAME + "CAP * LS :cap reply...\r\n");
    }

    std::string WELCOME_RPL(std::string user, std::string nick){
        std::cout << "USER: "
                  << user << "successfully registered to the irc network"
                  << std::endl;
        return (std::string("001 ") + nick +
                std::string(" :Welcome to the ft_irc network ") + nick +
                std::string("!") + user + std::string("@") + HOST +
                std::string("\r\n"));
    }
} // namespace irc

// -------------------------------------------------------------------------- //
