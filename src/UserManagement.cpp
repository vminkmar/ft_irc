#include "UserManagement.hpp"

void UserManagement::addUser(int socket_fd, std::string nickname, std::string username, UserPriviledge priviledge)
{
	User user(nickname, username, priviledge);
	users[socket_fd] = user;
}

void UserManagement::eraseUser(int socket_fd){
	std::map<int, User>::iterator it = users.find(socket_fd);
	users.erase(it);
}

void UserManagement::print(){
	for(std::map<int, User>::iterator it = users.begin(); it != users.end(); it ++)
		std::cout << it->first <<  "	" << it->second << std::endl;
}