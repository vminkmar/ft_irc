// -------------------------------------------------------------------------- //

#include "../include/Server.hpp" // needed for Server class

#include <iostream> // needed for std::cout, std::endl

/* LOG COLOURING */
#define YELLOW             "\033[33m"
#define PINK               "\033[95m"
#define CYAN               "\033[36m"
#define RED                "\033[31m"
#define WHITE              "\033[37m"
#define RESET              "\033[0m"
#define COLOUR_LOG         WHITE
#define COLOUR_INTERACTION WHITE
#define COLOUR_IN          CYAN
#define COLOUR_OUT         PINK
#define COLOUR_ERR         RED

void Server::log(t_str_c& message) const{
    std::cout << COLOUR_LOG << message << RESET << std::endl;
}

void Server::log_inc(int socket, t_str_c& message) const{
	t_str tmp = message.substr(0, message.find("\r\n"));
	if (tmp.empty() == true){
		return ;
	}
	std::cout << COLOUR_IN
              << "\nIncoming: " << tmp << " <-- socket#" << itostr(socket)
              << RESET << std::endl;
}

void Server::log_send(int socket, t_str_c& message) const{
    std::cout << COLOUR_OUT
              << "Sending: "<< message << " --> socket#" << itostr(socket)
              << RESET << std::endl;
}

void Server::log_err(t_str_c& message) const{
    std::cout << COLOUR_ERR << "Error: " << message << RESET << std::endl;
}

void Server::log_vector(t_str_c& name, t_vec_str_c& v) const{
    std::cout << COLOUR_LOG << "<" + name + "> ";
    for (t_vec_str_cit it = v.begin(); it != v.end(); ++it){
        std::cout << "[" << *it << "] ";
    }
    std::cout << RESET << std::endl;
}

// -------------------------------------------------------------------------- //
