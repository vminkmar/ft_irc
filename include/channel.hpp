
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>   // needed for std::string
#include "user.hpp" // needed for User class

class Channel
{
	private:

		std::string        m_name;
		std::string        m_topic;
		std::string        m_password;
		unsigned int       m_user_limit; /* unsigned int or just an int? */

		bool               m_invite_only;

		std::vector<User*> m_users; /* could be an alphabetical node list */


	public:

		void setName      (std::string const& newName);
		void setTopic     (std::string const& new_topic);
		void setPassword  (std::string const& new_password);
		void setUserLimit (unsigned int new_limit);

		std::string const& getName()      const;
		std::string const& getTopic()     const;
		std::string const& getPassword()  const;
		unsigned int       getUserLimit() const;
		std::string getUsers() const;
		
		void toogleInviteOnly();
		bool isInviteOnly() const;

		void addUser   (User const& u);
		void removeUser(User const& u);

		void sendBroadcast(std::string& message) const;

		void display() const; /* or an operator overload? */

		Channel(); /* not sure if needed yet */
		Channel(std::string const& name);
		Channel(std::string const& name,
				std::string const& password,
				std::string const& topic,
				unsigned int       user_limit,
				bool               invite_only); /* what should be there? */
		~Channel();

};

std::ostream& operator<<(std::ostream& os, Channel const & c);

#endif // CHANNEL_HPP

// maximum no of channels?
// in the code we just create an array/vector/list of channels?

// channel
	// info about channel
	// user reference

// create,delete,addUser,removeUser
// broadcast/message
// rights of users in a channel?
// display() for debugging
