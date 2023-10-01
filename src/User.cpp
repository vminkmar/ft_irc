#include "../include/User.hpp"

User::User() : m_nickname(""), m_username(""), m_privilege(USER){}
User::User(std::string nickname, std::string username,
           UserPrivilege privilege)
    : m_nickname(nickname), m_username(username), m_privilege(privilege) {}

User::User(const User &copy)
{
	this->m_nickname = copy.m_nickname;
	this->m_username = copy.m_username;
	this->m_privilege = copy.m_privilege;
}

User::~User() {}


void getUser(){}

std::string User::getNickname() const { return m_nickname;}
std::string User::getUsername() const { return m_username;}
UserPrivilege User::getPrivilege() const { return m_privilege;}


void User::setNickname(std::string nickname)
{
	this->m_nickname = nickname;
}

std::ostream &operator<<(std::ostream &os, const User &user)
{
    os << "Nickname: " << user.getNickname()
       << ", Username: " << user.getUsername() << ", privilege: " <<  user.getPrivilege();
    return os;
}