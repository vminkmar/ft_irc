#include "user.hpp"
User::User() : nickname(""), username(""), priviledge(USER){}
User::User(std::string nickname, std::string username,
           UserPriviledge priviledge)
    : nickname(nickname), username(username), priviledge(priviledge) {}

User::User(const User &copy)
{
	this->nickname = copy.nickname;
	this->username = copy.username;
	this->priviledge = copy.priviledge;
}

User::~User() {}


void getUser(){}

std::string User::getNickname() const { return nickname;}
std::string User::getUsername() const { return username;}
UserPriviledge User::getPriviledge() const { return priviledge;}


std::ostream &operator<<(std::ostream &os, const User &user)
{
    os << "Nickname: " << user.getNickname()
       << ", Username: " << user.getUsername() << ", Priviledge: " <<  user.getPriviledge();
    return os;
}
