
#include "../include/Channel.hpp" // needed for Channel class
#include <iostream>               // needed for std::cerr, std::cout, std::endl

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

Channel::Channel(std::string const& name) : m_name(name)
{
	print_log("name constructor called");
}

Channel::Channel()
{
    print_log("default constructor called");
}

Channel::~Channel()
{
    print_log("destructor called");
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> member functions */

std::string const& Channel::getName() const
{
	return m_name;
}

std::string const& Channel::getTopic() const
{
	return m_topic;
}

std::string const& Channel::getPassword() const
{
	return m_password;
}

unsigned int Channel::getUserLimit() const
{
	return m_user_limit;
}

std::string Channel::getUsers() const
{
	return ("Dummy");
}

bool Channel::isInviteOnly() const
{
	return m_invite_only;
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> operator overloads */

std::ostream& operator<<(std::ostream& os, Channel const & c)
{
	std::cout << "Channel:      " << c.getName() << "\n"
		      << "Topic:        " << c.getTopic() << "\n"
	          << "Password:     " << c.getPassword() << "\n"
			  << "User limit:   " << c.getUserLimit() << "\n"
			  << "isInviteOnly: " << c.isInviteOnly() << "\n"
			  << "Users:        " << c.getUsers();
	return os;
}

// -------------------------------------------------------------------------- //
