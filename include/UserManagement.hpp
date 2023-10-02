// -------------------------------------------------------------------------- //

#ifndef USERMANAGEMENT_HPP
#define USERMANAGEMENT_HPP

#include <string>  // needed for std::string
#include <map>     // needed for std::map
#include <vector>  // needed for std::vector

#include "Channel.hpp"       // needed for Channel class
#include "UserPrivilege.hpp" // needed for ENUM
#include "User.hpp"          // needed for User class
//#include "Responses.hpp" not needed right now

/* define maximum no. of channels? */

class UserManagement
{
	private:

		std::map <int, User>              m_users;
		std::map <std::string, Channel>   m_channels; /* change to std::map */

		bool checkForUser   (int socket)       const;
		bool checkForChannel(std::string name) const;

	public:

		void setNick(int socket, std::string parameter);
		void setUser(int socket, std::string user);

		std::string getNick               (int socket) const;
		std::string getNumberUsersAsString()           const;
		int         getNumberUsers        ()           const;
		
		void addUser(int                socket_fd,
                     std::string const& nickname,
					 std::string const& username);
		void eraseUser(int socket_fd);

		void addChannel(std::string name); /* add existing name check */
		void eraseChannel(std::string name); /* add non-existing check? */
		Channel const& getChannel(std::string name);
		void listChannels() const;

		void addUsertoChannel(int socket, UserPrivilege up, std::string channelName);

		
		void print(); /* old implementation */

		UserManagement();
		~UserManagement();
};

#endif // USERMANAGEMENT_HPP

// -------------------------------------------------------------------------- //
