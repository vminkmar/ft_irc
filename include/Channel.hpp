// -------------------------------------------------------------------------- //

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>   // needed for std::string
#include <map>      // needed for std::map <-- C11 would be unordered_map

#define USER_LIMIT_MAX 10

enum UserPrivilege
{
    USER,
    OPERATOR,
};

class Channel
{

    public:

        typedef std::map<int, UserPrivilege>    t_channel_users;
        typedef t_channel_users::iterator       t_channel_users_it;
        typedef t_channel_users::const_iterator t_channel_users_cit;

    private:

        std::string     m_name;
        std::string     m_topic;
        std::string     m_password;
        unsigned int    m_userLimit;
        t_channel_users m_users;
        
        bool            m_inviteOnly;
        bool            m_topicEditable;

    public:
  
        /* <------ constructors -----> */
        Channel           (std::string const& name);
        Channel           (Channel const&     src);
        Channel& operator=(Channel const&     src);
        Channel();
        ~Channel();

        /* <------ setters -----> */
        void setName     (std::string const& newName);
        void setTopic    (std::string const& newTopic);
        void setPassword (std::string const& newPassword);
        void setUserLimit(unsigned int       newLimit); 
        
        void toggleInviteOnly();
        void toggleTopicEditable();
        
        /* <------ getters -----> */
        std::string const&     getName()          const;
        std::string const&     getTopic()         const;
        std::string const&     getPassword()      const;
        unsigned int           getUserLimit()     const;
        t_channel_users const& getUserMap()       const;
        unsigned int           getNumberOfUsers() const;
        
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

#endif // CHANNEL_HPP

// -------------------------------------------------------------------------- //
