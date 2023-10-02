#ifndef USERMANAGEMENT_HPP
#define USERMANAGEMENT_HPP

#include <string>  // needed for std::string
#include <map>     // needed for std::map

#include "UserPrivilege.hpp" // needed for ENUM
#include "User.hpp"          // needed for User class
//#include "Responses.hpp" not needed right now

/* define maximum no. of channels? */

class UserManagement
{
	private:

		std::map<int, User> m_users; /* mb User* instead ? */
		
	public:

		void setNick(int socket, std::string parameter);

		std::string getNick               (int socket) const;
		std::string getNumberUsersAsString()           const;
		int         getNumberUsers        ()           const;
		
		void addUser(int                socket_fd,
                     std::string const& nickname,
					 std::string const& username,
					 UserPrivilege      privilege);
		void eraseUser(int socket_fd);

		void print();
};

#endif // USERMANAGEMENT_HPP
