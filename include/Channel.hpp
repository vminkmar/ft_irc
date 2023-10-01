// -------------------------------------------------------------------------- //

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "User.hpp" // needed for User class
#include <string>   // needed for std::string
#include <map>      // needed for std::map <-- C11 would be unordered_map

// @note mb define maximum no of channels?

class Channel
{
	private:

		std::string                    m_name;
		std::string                    m_topic;
		std::string                    m_password;
		unsigned int                   m_userLimit;
		bool                           m_inviteOnly;
		std::map<User*, UserPrivilege> m_users;

	public:

		void setName         (std::string const& newName);
		void setTopic        (std::string const& newTopic);
		void setPassword     (std::string const& newPassword);
		void setUserLimit    (unsigned int       newLimit);
		void toggleInviteOnly();

		std::string const& getName()      const;
		std::string const& getTopic()     const;
		std::string const& getPassword()  const;
		unsigned int       getUserLimit() const;
		std::string        getUsers()     const;
		bool               isInviteOnly() const;

		/* @note add ID checks in UM for adding and removing */
		void addUser   (User* u, UserPrivilege up); /* also set_privilege */
		void removeUser(User* u);

		/* @note could be done in UM too */
		/* void sendBroadcast(std::string& message) const; @needs impl. */

		Channel(std::string const& name);
		~Channel();

};

std::ostream& operator<<(std::ostream& os, Channel const & c);

#endif // CHANNEL_HPP

// -------------------------------------------------------------------------- //
