// -------------------------------------------------------------------------- //

#include "../include/UserManagement.hpp" // needed for UserManagement class
#include <iostream>                      // needed for std::cout, std::endl

#ifndef DEBUG
#define DEBUG 0
#endif // DEBUG

#define YELLOW "\033[33m"
#define RESET "\033[0m"

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> non-class functions */

static inline void print_log(std::string message){
  if (DEBUG){ 
    std::cerr << YELLOW << "User Management: " << message << RESET << std::endl;
  }
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> constructors */

UserManagement::UserManagement(){
    print_log("default constructor called");
}

UserManagement::~UserManagement(){
    print_log("destructor called");
}

// -------------------------------------------------------------------------- //
