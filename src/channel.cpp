
#include "../include/channel.hpp" // needed for Channel class
#include <iostream> // needed for std::cerr, std::cout, std::endl

#ifndef DEBUG
# define DEBUG 0
#endif // DEBUG

#define YELLOW "\033[33m"
#define RESET  "\033[0m"

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> non-class functions */

static inline void print_log(std::string message)
{
    if (DEBUG)
    {
        std::cerr << YELLOW << "Channel: "
                  << message << RESET << std::endl;
    }
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> constructors */

Channel::Channel()
{
    print_log("default constructor called");
}

Channel::~Channel()
{
    print_log("destructor called");
}

// -------------------------------------------------------------------------- //
