#include "user.hpp"
User::User() : m_nickname(""), m_username(""), m_priviledge(USER){}
User::User(std::string nickname, std::string username,
           UserPriviledge priviledge)
    : m_nickname(nickname), m_username(username), m_priviledge(priviledge) {}

User::User(const User &copy)
{
	this->m_nickname = copy.m_nickname;
	this->m_username = copy.m_username;
	this->m_priviledge = copy.m_priviledge;
}

User::~User() {}


void getUser(){}

std::string User::getNickname() const { return m_nickname;}
std::string User::getUsername() const { return m_username;}
UserPriviledge User::getPriviledge() const { return m_priviledge;}


void User::setNickname(std::string nickname)
{
	this->m_nickname = nickname;
}

std::ostream &operator<<(std::ostream &os, const User &user)
{
    os << "Nickname: " << user.getNickname()
       << ", Username: " << user.getUsername() << ", Priviledge: " <<  user.getPriviledge();
    return os;
}
