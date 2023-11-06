// -------------------------------------------------------------------------- //

#include "../include/class.Server.hpp" // needed for Server class

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    Server server;
    signal(SIGINT, Server::signal_handler);
  // server.getPortAndPasswd(argv);
    server.start();
}

// -------------------------------------------------------------------------- //
