// -------------------------------------------------------------------------- //

#include "../include/Server.hpp"

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  Server server;
  // server.getPortAndPasswd(argv);
  server.createSocket();
}

// -------------------------------------------------------------------------- //
