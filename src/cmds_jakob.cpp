// -------------------------------------------------------------------------- //

#include "../include/Server.hpp" // needed for Server class

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> commands jakob */

void Server::JOIN_RPL(int socket, std::string name) {
  std::string str = um.getNickname(socket) + "!" +
                    um.getUsername(socket) + "@" + HOST + " JOIN " +
                    name + " * :" + um.getUsername(socket) + "\r\n";
  um.appendToBuffer(str, socket, OUTPUT);
}

// -------------------------------------------------------------------------- //
