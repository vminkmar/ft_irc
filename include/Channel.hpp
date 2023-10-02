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
        std::map<int, UserPrivilege>   m_users;

        bool                           m_inviteOnly;
        bool                           m_topicEditable;
        bool                           m_channelKey;

	public:

		void setName         (std::string const& newName);
		void setTopic        (std::string const& newTopic);
		void setPassword     (std::string const& newPassword);
		void setUserLimit    (unsigned int       newLimit); 
		
        void toggleInviteOnly();
		void toggleTopicEditable();
		void toggleChannelKey();

		std::string const&                  getName()         const;
		std::string const&                  getTopic()        const;
		std::string const&                  getPassword()     const;
		unsigned int                        getUserLimit()    const;
        std::map<int, UserPrivilege> const& getUserMap()      const;

		bool               isInviteOnly()    const;
		bool               isTopicEditable() const;
		bool               isChannelKey()    const;
        
		/* also behaves like set_privilege */
		void addUser   (int socket, UserPrivilege up); 
		void removeUser(int socket);

		Channel(std::string const& name);
		Channel(Channel const& src);
		Channel& operator=(Channel const& src);
		Channel();
		~Channel();
};

std::ostream& operator<<(std::ostream& os, Channel const& c);

#endif // CHANNEL_HPP

// -------------------------------------------------------------------------- //
