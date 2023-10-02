// -------------------------------------------------------------------------- //

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "User.hpp" // needed for User class
#include <string>   // needed for std::string
#include <map>      // needed for std::map <-- C11 would be unordered_map

class Channel
{
	private:

		std::string                    m_name;
		std::string                    m_topic;
		std::string                    m_password;
		unsigned int                   m_userLimit;
		bool                           m_inviteOnly;
		bool                           m_topicEditable; /* new */
		bool                           m_channelKey; /* new */
		std::map<User*, UserPrivilege> m_users;

	public:

		void setName         (std::string const& newName);
		void setTopic        (std::string const& newTopic);
		void setPassword     (std::string const& newPassword);
		void setUserLimit    (unsigned int       newLimit); 
		void toggleInviteOnly();
		void toggleTopicEditable(); /* new */
		void toggleChannelKey(); /* new */

		std::string const& getName()         const;
		std::string const& getTopic()        const;
		std::string const& getPassword()     const;
		unsigned int       getUserLimit()    const;
		std::string        getUsers()        const;
		bool               isInviteOnly()    const;
		bool               isTopicEditable() const; /* new */
		bool               isChannelKey()    const; /* new */
        
		/* also behaves like set_privilege */
		void addUser   (User* u, UserPrivilege up); 
		void removeUser(User* u);

		/* void sendBroadcast(std::string& message) const; @needs impl. */

		Channel(std::string const& name);
		Channel(Channel const& src);
		Channel& operator=(Channel const& src);
		Channel();
		~Channel();

};

std::ostream& operator<<(std::ostream& os, Channel const& c);

#endif // CHANNEL_HPP

// -------------------------------------------------------------------------- //
