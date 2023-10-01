// -------------------------------------------------------------------------- //

#ifndef USER_HPP
#define USER_HPP

#include <string>            // needed for std::string
#include "UserPrivilege.hpp" // needed for ENUM

/* @note can prob remove the m_privilege + associated functions */

class User
{
	private:

		std::string   m_nickname;

		/* @note mb this should be const + cant be duplicates? */
		std::string   m_username;

		UserPrivilege m_privilege;

	public:

	  	void setNickname(std::string const& nickname);
	  	void setUsername(std::string const& username);

		std::string const& getNickname()  const;
		std::string const& getUsername()  const;
		UserPrivilege      getPrivilege() const;

		User();
		User(std::string nickname,
			 std::string username,
			 UserPrivilege privilege);
		User(const User &copy);
	  	~User();
};

std::ostream &operator<<(std::ostream &os, const User &user);

#endif // USER_HPP

// -------------------------------------------------------------------------- //
