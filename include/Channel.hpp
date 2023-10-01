// -------------------------------------------------------------------------- //

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "User.hpp" // needed for User class
#include <string>   // needed for std::string
#include <vector>   // needed for std::vector
#include <map>

// @note mb define maximum no of channels?
// @note rights of users in a channel?

class Channel
{
	private:

		std::string        m_name;
		std::string        m_topic;
		std::string        m_password;
		unsigned int       m_userLimit; /* unsigned int or just an int? */

		bool               m_inviteOnly;

		std::vector<User*> m_users; /* mb a map instead, with access rights */

	public:

		void setName      (std::string const& newName);
		void setTopic     (std::string const& newTopic);
		void setPassword  (std::string const& newPassword);
		void setUserLimit (unsigned int newLimit);

		std::string const& getName()      const;
		std::string const& getTopic()     const;
		std::string const& getPassword()  const;
		unsigned int       getUserLimit() const;
		std::string        getUsers()     const;

		void toggleInviteOnly();
		bool isInviteOnly() const;

		void addUser   (User* u);
		void removeUser(User const& u);
		/* add ID checks in UM */

		/* void sendBroadcast(std::string& message) const; @needs impl. */
		/* could be done in UM too */

		Channel(std::string const& name);
		~Channel();

};

std::ostream& operator<<(std::ostream& os, Channel const & c);

#endif // CHANNEL_HPP

// -------------------------------------------------------------------------- //
