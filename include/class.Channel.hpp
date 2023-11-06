// -------------------------------------------------------------------------- //

#ifndef CLASS_CHANNEL_HPP
# define CLASS_CHANNEL_HPP

#include "typedefs.hpp" // needed for Types, typedefs

class Channel
{

    private:

        t_str           m_name;
        t_str           m_topic;
        t_str           m_password;
        unsigned int    m_userLimit;
        t_channel_users m_users;
        
        bool            m_inviteOnly;
        bool            m_topicEditable;

    public:
  
        /* <------ constructors -----> */
        Channel           (t_str_c&       name);
        Channel           (Channel const& src);
        Channel& operator=(Channel const& src);
        Channel();
        ~Channel();

        /* <------ setters -----> */
        void setName     (t_str_c&     newName);
        void setTopic    (t_str_c&     newTopic);
        void setPassword (t_str_c&     newPassword);
        void setUserLimit(unsigned int newLimit); 
        
        void toggleInviteOnly();
        void toggleTopicEditable();
        
        /* <------ getters -----> */
        t_str_c&               getName()            const;
        t_str_c&               getTopic()           const;
        t_str_c&               getPassword()        const;
        unsigned int           getUserLimit()       const;
        t_channel_users const& getUserMap()         const;
        unsigned int           getNumberOfUsers()   const;
        int                    getFirstUserSocket() const;
        
        bool isInviteOnly()    const;
        bool isTopicEditable() const;
        bool isChannelKey()    const;
        bool isFull()          const;
        bool hasOperator()     const;
        
        /* <------ member functions -----> */
        void addUser   (int socket, UserPrivilege up); 
        void eraseUser (int socket);
        bool isMember  (int socket) const;
        bool isOperator(int socket) const;

};

std::ostream& operator<<(std::ostream& os, Channel const& c);

#endif // CLASS_CHANNEL_HPP

// -------------------------------------------------------------------------- //
