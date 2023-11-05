// -------------------------------------------------------------------------- //

#include "../include/class.Server.hpp"

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    Server server;
    signal(SIGINT, Server::signal_handler);
  // server.getPortAndPasswd(argv);
    server.start();
}

// -------------------------------------------------------------------------- //
