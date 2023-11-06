// -------------------------------------------------------------------------- //

#include "../include/class.Server.hpp" // needed for Server class

int main(int argc, char *argv[]) {
    (void)argc;
    Server server;
    signal(SIGINT, Server::signal_handler);
    server.start(argc, argv);
}

// -------------------------------------------------------------------------- //
