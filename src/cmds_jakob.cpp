// -------------------------------------------------------------------------- //

#include "../include/Server.hpp" // needed for Server class

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> commands jakob */

void Server::JOIN_RPL(int socket, std::string name) {
  std::string str = um.getNick(socket) + "!" +
                    um.getUser(socket) + "@" + HOST + " JOIN " +
                    name + " * :" + um.getUser(socket) + "\r\n";
  um.appendToBuffer(str, socket, OUTPUT);
}

// -------------------------------------------------------------------------- //
